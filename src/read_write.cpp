#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"

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
}