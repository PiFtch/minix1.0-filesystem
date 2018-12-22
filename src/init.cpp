#include "../include/fs.h"
#include "../include/debug.h"

/* 全局变量 */
d_inode inode[INODE_COUNT];     // inode缓存
unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE];    // inode bitmap缓存
unsigned char block_bitmap[BLOCK_BITMAP_COUNT*BLOCK_SIZE];    // block bitmap缓存
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
}

// 读入inode bitmap到缓存
void read_inode_bitmap(FILE *fd) {
    fseek(fd, INODE_BITMAP_START, SEEK_SET);
    fread(inode_bitmap, 1, INODE_BITMAP_COUNT*BLOCK_SIZE/8, fd);
}

// 读入block bitmap到缓存
void read_block_bitmap(FILE *fd) {
    fseek(fd, BLOCK_BITMAP_START, SEEK_SET);
    fread(&block_bitmap, 1, BLOCK_BITMAP_COUNT*BLOCK_SIZE/8, fd);
}

// 读入inode blocks到缓存
void read_inodes(FILE *fd) {
    fseek(fd, INODE_START, SEEK_SET);
    fread(inode, 1, SIZE_OF_D_INODE*INODE_COUNT, fd);
}

void init(FILE *fd) {
    read_super_block(fd);
    read_inode_bitmap(fd);
    read_block_bitmap(fd);
    read_inodes(fd);
    change_dir(fd, 0);
}