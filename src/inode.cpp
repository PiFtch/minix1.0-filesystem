/**
 * inode.cpp
 * 包含管理inode相关函数
 * show_inode
 * find_empty_inode
 * set_inode_bitmap -- 应该移至bitmap
 * set_inode
 * */

#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"

/* 显示inode缓冲区中某一inode的信息 */
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

/* 将某一个inode对应的位图位置1 */
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

/* 设置某一inode的内容 */
/**
 * dir -- 是否是目录
 * i_block -- 占用的磁盘块
 * */
void set_inode(int i_inode, bool dir, unsigned short i_block) {
    if (dir)
        inode[i_inode].i_mode = I_MODE_DIR;
    else
        inode[i_inode].i_mode = I_MODE_NORMAL;

    inode[i_inode].i_uid = I_UID;
    if (dir) {
        inode[i_inode].i_size = DIR_LENGTH * 2;
    } else {
        inode[i_inode].i_size = strlen(block_buffer) + 1;
    }

    inode[i_inode].i_time = time(NULL);

    /* 暂时不考虑写入超过1KB的数据 */
    inode[i_inode].i_zone[0] = i_block;
}