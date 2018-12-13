#include "../include/fs.h"
#include "../include/debug.h"

extern struct d_super_block super_block;
extern d_inode inode[32];
extern unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE/8];
extern struct dir current_dir;


extern void read_super_block(FILE *);
/*
void read_super_block(FILE *fd) {
    fseek(fd, D_SUPER_BLOCK_START, SEEK_SET);
    fread(&super_block, 1, SIZE_OF_D_SUPER_BLOCK, fd);
    cout << "super block:\n";
    cout << "s_nzones: " << super_block.s_nzones << '\n';
    cout << "s_imap_blocks: " << super_block.s_imap_blocks << '\n';
    cout << "s_zmap_blocks: " << super_block.s_zmap_blocks << '\n';
    cout << "s_firstdatazone: " << super_block.s_firstdatazone << '\n';
    cout << "s_log_zone_size: " << super_block.s_log_zone_size << '\n';
    cout << "s_max_size: " << super_block.s_max_size << '\n';
    cout << "s_magic: " << super_block.s_magic << '\n';
    cout << endl;
}
*/

extern void read_inode_bitmap(FILE *);
/*
void read_inode_bitmap(FILE *fd) {
    fseek(fd, INODE_BITMAP_START, SEEK_SET);
    fread(inode_bitmap, 1, INODE_BITMAP_COUNT*BLOCK_SIZE/8, fd);

    int count = (INODE_COUNT+1) % 8;
    if (count != 0) {
        count = (INODE_COUNT+1)/8 + 1;
    } else {
        count = (INODE_COUNT+1)/8;
    }

    cout << "inode count: " << INODE_COUNT << endl;
    for (int i = 0; i < count; i++) {
        // printf("%08x ", inode_bitmap[i]);    
        // if (i % 16 == 0)
        //     cout << endl;
        cout << bitset<8>(inode_bitmap[i]) << " ";
        if ((i+1)%16 == 0)
            cout << endl;
    }
    cout << endl;
}
*/


extern void read_inodes(FILE *);
/*
void read_inodes(FILE *fd) {
    fseek(fd, INODE_START, SEEK_SET);
    fread(inode, 1, SIZE_OF_D_INODE*INODE_COUNT, fd);

    int count = (INODE_COUNT+1) % 8;
    if (count != 0) {
        count = (INODE_COUNT+1)/8 + 1;
    } else {
        count = (INODE_COUNT+1)/8;
    }

    int c = -1;
    for (int i = 0; i < count; i++) {
        for (unsigned int j = 1; j <= (1<<7); j <<= 1) {
            unsigned char res = inode_bitmap[i] & j;
            if (res) {
                
                int index = log((int)res)/log(2) + c;
                if (index != -1 && index < INODE_COUNT) {
                    cout << "inode: " << index << endl;
                    cout << "i_mode: " << inode[index].i_mode << endl;
                    cout << "i_uid: " << inode[index].i_uid << endl;
                    cout << "i_size: " << inode[index].i_size << endl;
                    cout << "i_time: " << inode[index].i_time << endl;
                    cout << "i_gid: " << inode[index].i_gid << endl;
                    cout << "i_nlinks: " << inode[index].i_nlinks << endl;
                    cout << "i_zone: ";
                    for (int z = 0; z < 9; z++) {
                        cout << inode[index].i_zone[z] << ' ';
                    }
                    cout << endl;
                }
            }
            // getchar();
            // cout << (unsigned int)j << endl;
        }
        c += 8;
    }
}
*/

/* show() 显示文件内容 */
void show(FILE *fd, int i_inode) {
    char buf[BLOCK_SIZE];
    long offset;
    for (int z = 0; z < 9; z++) {
        // offset = inode[i_inode].i_zone[z];
        if (inode[i_inode].i_zone[z] != 0) {
            offset = inode[i_inode].i_zone[z] * BLOCK_SIZE;
            fseek(fd, offset, SEEK_SET);
            fread(buf, 1, 1024, fd);
            cout << buf;
            // cout << "show 3" << endl;
        } else {
            break;
        }
    }
    // offset = inode[i_inode];
}

/* 显示当前目录 */
void ls(FILE *fd, int i_inode) {
    char filename[32][14];      // 每个BLOCK（1024）可放32个目录项
    long offset;
    unsigned short tmp_inode;
    for (int z = 0; z < 9; z++) {
        int index_filename = 0;
        if (inode[i_inode].i_zone[z] != 0) {
            // cout << inode[i_inode].i_zone[z] << endl;
            offset = inode[i_inode].i_zone[z] * BLOCK_SIZE;
            fseek(fd, offset, SEEK_SET);
            for (int i = 0; i < 32; i++) {
                fread(&tmp_inode, 2, 1, fd);
                // cout << "tmp_inode is " << tmp_inode << endl;
                if (tmp_inode != 0) {
                    fread(filename[index_filename++], 1, 14, fd);
                    fseek(fd, 16, SEEK_CUR);
                } else {
                    fseek(fd, 14 + 16, SEEK_CUR);
                }
            }
            for (int i = 0; i < index_filename; i++) {
                cout << filename[i] << endl;
            }
        } else break;
        
    }
    
}

/* 将文件名转换为inode号 */
/* 目前只支持在current_dir中搜索 */
int namei(char *name) {
    char tmp_filename[14] = {0};
    char c;
    int i = 0;
    while (c = *(name + i)) {
        tmp_filename[i++] = c;
        // cout << c;
    }
    // cout << tmp_filename << endl;
    for (i = 0; i < 32; i++) {
        if (strcmp((current_dir.filename[i]), tmp_filename) == 0) {
            // cout << "match" << endl;
            return current_dir.inode[i];
        }
    }
    return -1;
}

void change_dir(FILE *fd, int i_inode) {
    current_dir.i_inode = i_inode;
    // clear current_dir
    for (int i = 0; i < 32; i++) {
        current_dir.inode[i] = 0;
        // current_dir.filename[i] = {0};
    }

    long offset;
    unsigned short tmp_inode;
    int index_file = 0;
    
    for (int z = 0; z < 9; z++) {
        if (inode[i_inode].i_zone[z] != 0) {
            offset = inode[i_inode].i_zone[z] * BLOCK_SIZE;
            fseek(fd, offset, SEEK_SET);
            for (int i = 0; i < 32; i++) {
                fread(&tmp_inode, 2, 1, fd);
                if (tmp_inode != 0) {
                    current_dir.inode[index_file] = tmp_inode;
                    fread(current_dir.filename[index_file++], 1, 14, fd);
                    fseek(fd, 16, SEEK_CUR);
                } else {
                    fseek(fd, 14 + 16, SEEK_CUR);
                }
            }
        }
    }
}

void show_current_dir() {
    cout << "current directory inode: " << current_dir.i_inode << endl;
    cout << setw(3) << "inode" << setw(15) << "file name" << setw(10) << "size" << endl;
    int count = 0;
    for (int i = 0; i < 32; i++) {
        if (current_dir.inode[i] != 0) {
            cout << setw(3) << current_dir.inode[i] << setw(15) << current_dir.filename[i] << setw(10) << inode[current_dir.inode[i]-1].i_size << endl; 
            count++;
        }
    }
    cout << "total count: " << count << endl;
}

int main() {
    unsigned char buf[1024];
    FILE *fd = fopen(FILESYSTEM, "r+w+b");

    read_super_block(fd);
    read_inode_bitmap(fd);
    read_inodes(fd);
    show(fd, 3);
    show(fd, 4);
    cout << "ls" << endl;
    ls(fd, 0);
    change_dir(fd, 0);
    show_current_dir();
    // char filename[14] = "test3";
    // cout << namei(filename) << endl;
    char filename[14] = "test_dir";
    cout << "cd test_dir" << endl;
    change_dir(fd, namei(filename)-1);
    show_current_dir();
    show(fd, namei(filename)-1);
    cout << "cd test_dir2" << endl;
    char filename3[14] = "test_dir2";
    change_dir(fd, namei(filename3)-1);
    show_current_dir();
    char filename2[14] = "..";
    change_dir(fd,namei(filename2)-1);
    show_current_dir();
    change_dir(fd, namei(filename2)-1);
    show_current_dir();
/*
    fseek(fd, 4096, SEEK_SET);
    fread(inode, 1, 1024, fd);
    fseek(fd, 5*1024, SEEK_SET);
    unsigned short first_inode1;
    unsigned short first_inode2;
    char filename1[14];
    char filename2[14];
    fread(&first_inode1, 1, 2, fd);
    fread(filename1, 1, 14, fd);
    fseek(fd, 5*1024 + 32, SEEK_SET);
    fread(&first_inode2, 1, 2, fd);
    fread(filename2, 1, 14, fd);

    unsigned short first_inode3;
    char filename3[14];
    fseek(fd, 5*1024 + 64, SEEK_SET);
    fread(&first_inode3, 1, 2, fd);
    fread(&filename3, 1, 14, fd);

    cout << "first inode1: " << first_inode1 << endl;
    cout << "filename1: " << filename1 << endl;
    cout << "first inode2: " << first_inode2 << endl;
    cout << "filename2: " << filename2 << endl;
    cout << "first inode3: " << first_inode3 << endl;
    cout << "filename3: " << filename3 << endl;

    cout << "sizeof d_inode: " << sizeof(d_inode) << endl;
    
    for (int i = 0; i < 20; i++) {
        printf("%02x ", *((unsigned char *)&super_block + i));
    }
    cout << endl;
    cout << "s_ninodes: " << super_block.s_ninodes << endl;
    cout << "s_nzones: " << super_block.s_nzones << endl;
    cout << "s_imap_blocks: " << super_block.s_imap_blocks << endl;
    cout << "s_zmap_blocks: " << super_block.s_zmap_blocks << endl;
    cout << "s_firstdatazone: " << super_block.s_firstdatazone << endl;
    cout << "s_log_zone_size: " << super_block.s_log_zone_size << endl;
    cout << "s_max_size: " << super_block.s_max_size << endl;
    cout << "s_magic: " << super_block.s_magic << endl;
    // printf("%02x", (unsigned char)buf[16]);

    cout << "inodes: " << endl;
    for (int i = 0; i < 32; i++) {
        cout << "inode: " << i << endl;
        cout << "i_mode: " << inode[i].i_mode << endl;
        cout << "i_uid: " << inode[i].i_uid << endl;
        cout << "i_size: " << inode[i].i_size << endl;
        cout << "i_time: " << inode[i].i_time << endl;
        cout << "i_gid: " << inode[i].i_gid << endl;
        cout << "i_nlinks: " << inode[i].i_nlinks << endl;
        cout << "i_zone: ";
        for (int j = 0; j < 9; j++) {
            cout << inode[i].i_zone[j] << ' ';
        }
        cout << endl;
    }
    */
}