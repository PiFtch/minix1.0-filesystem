#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"
/*
extern struct d_super_block super_block;
extern d_inode inode[32];
extern unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE/8];
extern unsigned char block_bitmap[BLOCK_BITMAP_COUNT*BLOCK_SIZE/8];    // block bitmap缓存
extern struct dir current_dir;
extern char current_path[100];
*/
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

void show_block_bitmap() {

    for (int i = 0; i <= (AVAILABLE_BLOCK_COUNT+1)/8; i++) {
        cout << bitset<8>(block_bitmap[i]) << " ";
        if ((i+1)%16 == 0)
            cout << endl;
    }
    cout << endl;
}