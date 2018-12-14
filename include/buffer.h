#ifndef BUFFER_H
#define BUFFER_H
extern struct d_super_block super_block;
extern d_inode inode[32];
extern unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE/8];
extern unsigned char block_bitmap[BLOCK_BITMAP_COUNT*BLOCK_SIZE/8];    // block bitmap缓存
extern struct dir current_dir;
extern char current_path[100];
extern char block_buffer[BLOCK_SIZE];
#endif
//end