#include "../include/fs.h"
#include "../include/debug.h"

extern struct d_super_block super_block;
extern d_inode inode[32];
extern unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE/8];
extern struct dir current_dir;

extern char current_path[100];


extern void read_super_block(FILE *);
extern void read_inode_bitmap(FILE *);
extern void read_inodes(FILE *);
extern void read_block_bitmap(FILE *);
extern void init(FILE *);

extern void read_block(FILE *fd, int i_block);
extern void show_block_buffer();

extern void show_inode_bitmap();
extern void show_inode(int i_inode);
extern int find_empty_inode();
/* show() 显示文件内容 */
void show(FILE *fd, int i_inode) {
    char buf[BLOCK_SIZE];
    long offset;
    for (int z = 0; z < 9; z++) {
        // offset = inode[i_inode].i_zone[z];
        if (inode[i_inode].i_zone[z] != 0) {
            offset = inode[i_inode].i_zone[z] * BLOCK_SIZE;
            fseek(fd, offset, SEEK_SET);
            fread(buf, 1, 1024, fd);
            cout << buf;
            // cout << "show 3" << endl;
        } else {
            break;
        }
    }
    // offset = inode[i_inode];
}

extern void ls(FILE *, int);


/* 将文件名转换为inode号 */
/* 目前只支持在current_dir中搜索 */
int namei(char *name) {
    char tmp_filename[14] = {0};
    char c;
    int i = 0;
    while (c = *(name + i)) {
        tmp_filename[i++] = c;
        // cout << c;
    }
    // cout << tmp_filename << endl;
    for (i = 0; i < 32; i++) {
        if (strcmp((current_dir.filename[i]), tmp_filename) == 0) {
            // cout << "match" << endl;
            return current_dir.inode[i];
        }
    }
    return -1;
}
extern void change_dir(FILE *, int);
extern void show_current_dir();


int main() {
    unsigned char buf[1024];
    FILE *fd = fopen(FILESYSTEM, "r+w+b");

    /*
    read_super_block(fd);
    read_inode_bitmap(fd);
    read_inodes(fd);
    show(fd, 3);
    show(fd, 4);
    cout << "ls" << endl;
    ls(fd, 0);
    change_dir(fd, 0);
    show_current_dir();
    // char filename[14] = "test3";
    // cout << namei(filename) << endl;
    char filename[14] = "test_dir";
    cout << "cd test_dir" << endl;
    change_dir(fd, namei(filename)-1);
    show_current_dir();
    show(fd, namei(filename)-1);
    cout << "cd test_dir2" << endl;
    char filename3[14] = "test_dir2";
    change_dir(fd, namei(filename3)-1);
    show_current_dir();
    char filename2[14] = "..";
    change_dir(fd,namei(filename2)-1);
    show_current_dir();
    change_dir(fd, namei(filename2)-1);
    show_current_dir();
    */
    system("cat /mnt/Workspace/minix/mkfs.txt");
    init(fd);
    show_current_dir();
    show_inode_bitmap();
    for (int i = 0; i < INODE_COUNT; i++) {
        show_inode(i);
    }
    cout << time(NULL) << endl;
    cout << find_empty_inode() << endl;
    // show_inode(0);
    char command[50];
    char args[50];
    
    /* loop */
    /*
    while (true) {
        cout << current_path << '$';
        // cin >> command;
        scanf("%s", command);
        if (strcmp(command, "ls") == 0) {
            show_current_dir();
            continue;
        }
        if (strcmp(command, "cd") == 0) {
            // cin >> args;
            scanf("%s", args);
            change_dir(fd, namei(args)-1);
            continue;
        }

        if (strcmp(command, "hexdump") == 0) {
            system("hexdump ../filesystem.img");
            continue;
        }
    }
    
    */
}