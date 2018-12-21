#include "../include/fs.h"
#include "../include/debug.h"
#include "../include/buffer.h"

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

extern unsigned short find_empty_block();

void write(FILE *fd, char name[], char content[]);
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
    cout << "create a new disk image with minix1.0 filesystem?(y/n) ";
    if (getchar() == 'y') {
        system("dd if=/dev/zero of=../filesystem.img bs=512 count=72");
        system("mkfs.minix -1 ../filesystem.img");
    }
    unsigned char buf[1024];
    FILE *fd = fopen(FILESYSTEM, "r+w+b");

    system("cat /mnt/Workspace/minix/mkfs.txt");
    init(fd);
    show_current_dir();
    show_inode_bitmap();
    cout << find_empty_inode() << endl;
    cout << find_empty_block() << endl;
    char command[50];
    char args[50];
    char name[14] = "abc", content[100] = "hellohello";
    
    
    /* loop */
    while (true) {
        cout << current_path << '$';
        scanf("%s", command);
        if (strcmp(command, "exit") == 0) {
            return 0;
        }

        if (strcmp(command, "cat") == 0) {
            scanf("%s", args);
            show(fd, namei(args)-1);
            continue;
        }

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

        if (strcmp(command, "write") == 0) {
            write(fd, name, content);
        }

        if (strcmp(command, "mkfs") == 0) {
            fclose(fd);
            system("dd if=/dev/zero of=../filesystem.img bs=512 count=72");
            system("mkfs.minix -1 ../filesystem.img");
            fd = fopen(FILESYSTEM, "r+w+b");
            init(fd);
            continue;
        }
    }
    
    
}