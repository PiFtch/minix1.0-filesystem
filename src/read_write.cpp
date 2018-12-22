#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"

extern int find_empty_inode();
extern void set_inode_bitmap(int i_inode);
extern void set_inode(int i_inode, bool dir, unsigned short i_block);
unsigned short find_empty_block();
void set_block_bitmap(unsigned short i_block);

void sync(FILE *, unsigned short);
/* 在当前目录下创建新文件 */

extern void show_inode(int i_inode);
extern void show_inode_bitmap();
extern void show_block_bitmap();
extern void change_dir(FILE *fd, int i_inode);

void write(FILE *fd, char name[], char content[]) {
    if (strlen(name) >= 14) {
        cout << "file name length cannot surpass 14 bytes!" << endl;
        return;
    }
    
    unsigned int length = strlen(content) + 1;
    if (length > BLOCK_SIZE) {
        cout << "file length cannot surpath 1 block(" << BLOCK_SIZE << "B) for now!" << endl;
        return;
    }

    cout << "content length: " << length << endl;

    cout << "before: inode bitmap: ";
    show_inode_bitmap();
    cout << "before: block bitmap: ";
    show_block_bitmap();
    strcpy(block_buffer, content);
    cout << "copy content to block_buffer: " << block_buffer << endl;
    int i_block = find_empty_block();
    cout << "find empty block: " << i_block << endl;
    int i_inode = find_empty_inode();
    cout << "find empty inode: " << i_inode << endl;
    
    set_inode(i_inode, false, i_block);
    
    show_inode(i_inode);
    set_inode_bitmap(i_inode);
    cout << "after: inode bitmap: ";
    show_inode_bitmap();
    set_block_bitmap(i_block);
    cout << "after: block bitmap: ";
    show_block_bitmap();
    
    // cout << "current direct i_inode: " << current_dir.i_inode << endl;
    show_inode(current_dir.i_inode);
    inode[current_dir.i_inode].i_time = inode[i_inode].i_time;
    inode[current_dir.i_inode].i_size += 32;
    show_inode(current_dir.i_inode);
    // inode[current_dir.i_inode]
    cout << bitset<8>(current_dir.block[0][0]) << endl;
    cout << bitset<8>(current_dir.block[0][1]) << endl;
    for (int i = 0; i < 32; i++) {
        if (current_dir.block[i][0] == 0 && current_dir.block[i][1] == 0) {
            cout << "flag" << endl;
            
            unsigned char low, high;
            low = (unsigned short)(i_inode + 1) & 0x00ff;
            cout << "low: " << low << endl;
            high = (unsigned short)(i_inode + 1) & 0xff00;
            cout << "high: " << high << endl;
            current_dir.block[i][0] = low;
            cout << bitset<8>(current_dir.block[i][0]) << endl;
            current_dir.block[i][1] = high;
            cout << bitset<8>(current_dir.block[i][1]) << endl;
            strcpy(((char *)current_dir.block[i] + 2), name);
            cout << "name: " << (char *)current_dir.block[i] + 2 << endl;
            break;
        }
    }

    sync(fd, i_block);
    change_dir(fd, current_dir.i_inode);
}

void mkdir(FILE *fd, char name[]) {
    if (strlen(name) >= 14) {
        cout << "directory name length cannot surpass 14 bytes!" << endl;
        return;
    }

    cout << "before: inode bitmap: ";
    show_inode_bitmap();
    cout << "before: block bitmap: ";
    show_block_bitmap();
    int i_inode = find_empty_inode();
    cout << "find empty inode: " << i_inode << endl;
    int i_block = find_empty_block();
    cout << "find empty block: " << i_block << endl;

    *(unsigned short *)block_buffer = i_inode + 1;
    strcpy(block_buffer + 2, ".");
    *(unsigned short *)(block_buffer + DIR_LENGTH) = current_dir.i_inode + 1;
    strcpy(block_buffer + DIR_LENGTH + sizeof(unsigned short), "..");
    set_inode(i_inode, true, i_block);

    int cur_dir_num = inode[current_dir.i_inode].i_size / DIR_LENGTH;
    current_dir.inode[cur_dir_num] = i_inode + 1;
    *(unsigned short *)current_dir.block[cur_dir_num] = i_inode + 1;
    strncpy((char *)current_dir.block[cur_dir_num] + sizeof(unsigned short), name, 14);
    strncpy(current_dir.filename[cur_dir_num], name, 14);

    inode[current_dir.i_inode].i_size += DIR_LENGTH;
    inode[current_dir.i_inode].i_time = time(NULL);
    /* block索引未更新 */
    
    
    show_inode(i_inode);
    set_inode_bitmap(i_inode);
    cout << "after: inode bitmap: ";
    show_inode_bitmap();
    set_block_bitmap(i_block);
    cout << "after: block bitmap: ";
    show_block_bitmap();

    show_inode(current_dir.i_inode);
    sync(fd, i_block);
    change_dir(fd, current_dir.i_inode);
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

    // 写回目录块
    fseek(fd, inode[current_dir.i_inode].i_zone[0] * BLOCK_SIZE, SEEK_SET);
    fwrite(current_dir.block[0], 32, 32, fd);
}