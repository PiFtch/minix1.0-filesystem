#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"

// extern unsigned char block_buffer[BLOCK_SIZE];

// 读取一整块
void read_block(FILE *fd, int i_block) {
    // unsigned char *buf;
    cout << "enter read_block()" << endl;
    long offset = i_block * BLOCK_SIZE;
    fseek(fd, offset, SEEK_SET);
    fread(block_buffer, 1, BLOCK_SIZE, fd);
}

// 显示block缓存内容
void show_block_buffer() {
    // system("echo hello");
    /*
    for (int i = 0; i < BLOCK_SIZE; i++) {
        
        if (block_buffer[i] != 0)
            cout << (char)block_buffer[i];
        else
            cout << "nb";
        if ((i+1) % 32 == 0)
            cout << endl;
    }
    */
    for (int i = 0; i < BLOCK_SIZE; i++) {
        cout << hex << (unsigned char)block_buffer[i] << ' ';
        // printf("%02x ", block_buffer+i);
        if ((i+1) % 32 == 0)
            cout << endl;
    }
    cout << endl;
}