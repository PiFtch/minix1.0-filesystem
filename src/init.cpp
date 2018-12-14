#include "../include/fs.h"
#include "../include/debug.h"

/* 全局变量 */
d_inode inode[INODE_COUNT];     // inode缓存
unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE/8];    // inode bitmap缓存
unsigned char block_bitmap[BLOCK_BITMAP_COUNT*BLOCK_SIZE/8];    // block bitmap缓存
dir current_dir;    // 当前目录
d_super_block super_block;  // 超级块缓存
char current_path[100] = {0};
char block_buffer[BLOCK_SIZE];
/* 全局变量 */

extern void change_dir(FILE *fd, int i_inode);

// 读入超级块到缓存
void read_super_block(FILE *fd) {
    fseek(fd, D_SUPER_BLOCK_START, SEEK_SET);
    fread(&super_block, 1, SIZE_OF_D_SUPER_BLOCK, fd);
    /*
    cout << "super block:\n";
    cout << "s_nzones: " << super_block.s_nzones << '\n';
    cout << "s_imap_blocks: " << super_block.s_imap_blocks << '\n';
    cout << "s_zmap_blocks: " << super_block.s_zmap_blocks << '\n';
    cout << "s_firstdatazone: " << super_block.s_firstdatazone << '\n';
    cout << "s_log_zone_size: " << super_block.s_log_zone_size << '\n';
    cout << "s_max_size: " << super_block.s_max_size << '\n';
    cout << "s_magic: " << super_block.s_magic << '\n';
    cout << endl;
    */
}

// 读入inode bitmap到缓存
void read_inode_bitmap(FILE *fd) {
    fseek(fd, INODE_BITMAP_START, SEEK_SET);
    fread(inode_bitmap, 1, INODE_BITMAP_COUNT*BLOCK_SIZE/8, fd);
    /*
    int count = (INODE_COUNT+1) % 8;
    if (count != 0) {
        count = (INODE_COUNT+1)/8 + 1;
    } else {
        count = (INODE_COUNT+1)/8;
    }

    cout << "inode count: " << INODE_COUNT << endl;
    for (int i = 0; i < count; i++) {
        cout << bitset<8>(inode_bitmap[i]) << " ";
        if ((i+1)%16 == 0)
            cout << endl;
    }
    cout << endl;
    */
}

// 读入block bitmap到缓存
void read_block_bitmap(FILE *fd) {
    fseek(fd, BLOCK_BITMAP_START, SEEK_SET);
    fread(&block_bitmap, 1, BLOCK_BITMAP_COUNT*BLOCK_SIZE/8, fd);

    /* test cout */
    /*
    int count = (BLOCK_COUNT+1) % 8;
    if (count != 0) {
        count = (BLOCK_COUNT+1)/8 + 1;
    } else {
        count = (BLOCK_COUNT+1)/8;
    }
    cout << "block count: " << BLOCK_COUNT << endl;
    for (int i = 0; i < count; i++) {
        cout << bitset<8>(block_bitmap[i]) << " ";
        if ((i+1)%16 == 0)
            cout << endl;
    }
    cout << endl;
    */
}

// 读入inode blocks到缓存
void read_inodes(FILE *fd) {
    fseek(fd, INODE_START, SEEK_SET);
    fread(inode, 1, SIZE_OF_D_INODE*INODE_COUNT, fd);
    /*
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
                    cout << "inode: " << index+1 << endl;
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
    */
}

void init(FILE *fd) {
    read_super_block(fd);
    read_inode_bitmap(fd);
    read_block_bitmap(fd);
    read_inodes(fd);
    change_dir(fd, 0);
}