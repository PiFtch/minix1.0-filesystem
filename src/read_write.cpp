#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"

extern int find_empty_inode();
extern void set_inode_bitmap(int i_inode);
extern void set_inode(int i_inode, bool dir);

/* 在当前目录下创建新文件 */
void write(char name[], char content[]) {
    if (strlen(name) >= 14) {
        cout << "file name length cannot surpass 14 bytes!" << endl;
        return;
    }
    
    unsigned int length = strlen(content) + 1;
    if (length > BLOCK_SIZE) {
        cout << "file length cannot surpath 1 block(" << BLOCK_SIZE << "B) for now!" << endl;
        return;
    }

    strcpy(block_buffer, content);

    int i_inode = find_empty_inode();
    set_inode(i_inode, false);

    set_inode_bitmap(i_inode);
}

void sync(FILE *fd, unsigned short nr_block) {
    // 写回inode bitmap
    fseek(fd, INODE_BITMAP_START, SEEK_SET);
    fwrite(inode_bitmap, 1, INODE_BITMAP_COUNT * BLOCK_SIZE, fd);

    // 写回block bitmap
    fseek(fd, BLOCK_BITMAP_START, SEEK_SET);
    fwrite(block_bitmap, 1, BLOCK_BITMAP_COUNT * BLOCK_SIZE, fd);

    // 写回inode
    fseek(fd, INODE_START, SEEK_SET);
    fwrite(inode, SIZE_OF_D_INODE, INODE_COUNT, fd);

    // 写回缓存块
    fseek(fd, nr_block * BLOCK_SIZE, SEEK_SET);
    fwrite(block_buffer, 1, BLOCK_SIZE, fd);
}