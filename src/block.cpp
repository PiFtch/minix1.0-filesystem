/**
 * block.cpp
 * 管理数据块相关函数
 * find_empty_block
 * set_block_bitmap -- 应移至bitmap.cpp
 * */

#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"

unsigned short find_empty_block() {
    int nr_block_bitmap_byte;
    /* 获取inode bitmap占用的字节数 */
    if ((AVAILABLE_BLOCK_COUNT + 1) % 8 == 0) {
        nr_block_bitmap_byte = (AVAILABLE_BLOCK_COUNT + 1) / 8;
    } else {
        nr_block_bitmap_byte = (AVAILABLE_BLOCK_COUNT + 1) / 8 + 1;
    }

    int c = -1;
    for (int i = 0; i < nr_block_bitmap_byte; i++) {

        for (unsigned short j = 1; j <= (1 << 7); j <<= 1) {
            unsigned char res = block_bitmap[i] & j;
            if (res == 0) { // res为0，表示找到了空闲inode
                int index = log((unsigned int)j) / log(2) + c;
                return index + 1 + 1 + INODE_BITMAP_COUNT + BLOCK_BITMAP_COUNT + INODE_BLOCK_COUNT;
            }
        }
        c += 8;
    }
    return -1;
}

void set_block_bitmap(unsigned short i_block) {
    
    if (i_block < 0 || i_block >= BLOCK_COUNT)
        return ;
    cout << "now setting block bitmap 6 as 1..." << endl;
    int nr_block_bitmap_byte = (i_block - 1 - 1 - INODE_BITMAP_COUNT - BLOCK_BITMAP_COUNT - INODE_BLOCK_COUNT + 1) / 8;
    cout << "nr_block_bitmap_byte: " << nr_block_bitmap_byte << endl;
    unsigned char temp;
    if (i_block - 1 - 1 - INODE_BITMAP_COUNT - BLOCK_BITMAP_COUNT - INODE_BLOCK_COUNT > 6) {
        cout << "iblock - 5 > 6" << endl;
        temp = (i_block - 1 - 1 - INODE_BITMAP_COUNT - BLOCK_BITMAP_COUNT - INODE_BLOCK_COUNT) - (nr_block_bitmap_byte - 1) * 8 - 6;
        block_bitmap[nr_block_bitmap_byte] += (1 << (temp - 1));
    } else {
        cout << "iblock - 5 <= 6" << endl;
        block_bitmap[nr_block_bitmap_byte] += ((unsigned)1 << (int)(i_block - 1 - 1 - INODE_BITMAP_COUNT - BLOCK_BITMAP_COUNT - INODE_BLOCK_COUNT + 1));
    }
}

void set_block() {
    
}