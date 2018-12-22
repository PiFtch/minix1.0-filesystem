/**
 * bitmap.cpp
 * 管理位图相关操作
 * 
 * */

#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"

/* 显示inode bitmap */
void show_inode_bitmap() {
    
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
    
}

/* 显示block bitmap */
void show_block_bitmap() {
    cout << "block count: " << BLOCK_COUNT << endl;
    for (int i = 0; i <= (AVAILABLE_BLOCK_COUNT+1)/8; i++) {
        cout << bitset<8>(block_bitmap[i]) << " ";
        if ((i+1)%16 == 0)
            cout << endl;
    }
    cout << endl;
}