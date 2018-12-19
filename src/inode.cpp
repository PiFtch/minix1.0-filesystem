#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"
/*
extern struct d_super_block super_block;
extern d_inode inode[32];
extern unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE/8];
extern struct dir current_dir;
extern char current_path[100];
*/
void show_inode(int i_inode) {
    if (i_inode == -1) {
        return;
    }
    cout << SIZE_OF_D_INODE << endl;
    cout << "i_inode: " << i_inode << " inode: " << i_inode+1 << endl;
    cout << "i_mode: " << bitset<16>(inode[i_inode].i_mode) << endl;
    cout << "i_uid: " << inode[i_inode].i_uid << endl;
    cout << "i_size: " << inode[i_inode].i_size << " bytes" << endl;
    cout << "i_time: " << inode[i_inode].i_time << endl;
    cout << "i_gid: " << inode[i_inode].i_gid << " (unused for now)" << endl;
    cout << "i_nlinks: " << inode[i_inode].i_nlinks << endl;
    cout << "i_zone: ";
    for (int z = 0; z < 9; z++) {
        cout << inode[i_inode].i_zone[z] << ' ';
    }
    cout << endl;

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

/* 找到内存中第一个空闲的inode，返回i_inode */
/* 如果没有空闲，返回-1 */
int find_empty_inode() {
    int nr_inode_bitmap_byte;
    /* 获取inode bitmap占用的字节数 */
    if ((INODE_COUNT + 1) % 8 == 0) {
        nr_inode_bitmap_byte = (INODE_COUNT + 1) / 8;
    } else {
        nr_inode_bitmap_byte = (INODE_COUNT + 1) / 8 + 1;
    }

    int c = -1;
    for (int i = 0; i < nr_inode_bitmap_byte; i++) {

        for (unsigned short j = 1; j <= (1 << 7); j <<= 1) {
            unsigned char res = inode_bitmap[i] & j;
            if (res == 0) { // res为0，表示找到了空闲inode
                int index = log((unsigned int)j) / log(2) + c;
                return index;
            }
        }
        c += 8;
    }
    return -1;
}

void set_inode_bitmap(int i_inode) {
    if (i_inode < 0 || i_inode >= INODE_COUNT)
        return ;
    int nr_inode_bitmap_byte = (i_inode + 1) / 8;
    unsigned char temp;
    if (i_inode > 6) {
        temp = i_inode - (nr_inode_bitmap_byte - 1) * 8 - 6;
        inode_bitmap[nr_inode_bitmap_byte] += (1 << (temp - 1));
    } else {
        inode_bitmap[nr_inode_bitmap_byte] += ((unsigned)1 << (i_inode + 1));
    }
}

void set_inode(int i_inode, bool dir, unsigned short i_block) {
    if (dir)
        inode[i_inode].i_mode = I_MODE_DIR;
    else
        inode[i_inode].i_mode = I_MODE_NORMAL;

    inode[i_inode].i_uid = I_UID;
    inode[i_inode].i_size = strlen(block_buffer) + 1;
    inode[i_inode].i_time = time(NULL);

    /* 暂时不考虑写入超过1KB的数据 */
    inode[i_inode].i_zone[0] = i_block;
}