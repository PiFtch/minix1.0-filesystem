#include "../include/fs.h"
#include "../include/debug.h"
extern struct d_super_block super_block;
extern d_inode inode[32];
extern unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE/8];
extern struct dir current_dir;

extern char current_path[100];

/* 显示当前目录 */
void ls(FILE *fd, int i_inode) {
    char filename[32][14];      // 每个BLOCK（1024）可放32个目录项
    long offset;
    unsigned short tmp_inode;
    for (int z = 0; z < 9; z++) {
        int index_filename = 0;
        if (inode[i_inode].i_zone[z] != 0) {
            // cout << inode[i_inode].i_zone[z] << endl;
            offset = inode[i_inode].i_zone[z] * BLOCK_SIZE;
            fseek(fd, offset, SEEK_SET);
            for (int i = 0; i < 32; i++) {
                fread(&tmp_inode, 2, 1, fd);
                // cout << "tmp_inode is " << tmp_inode << endl;
                if (tmp_inode != 0) {
                    fread(filename[index_filename++], 1, 14, fd);
                    fseek(fd, 16, SEEK_CUR);
                } else {
                    fseek(fd, 14 + 16, SEEK_CUR);
                }
            }
            for (int i = 0; i < index_filename; i++) {
                cout << filename[i] << endl;
            }
        } else break;
        
    }
    
}


void change_dir(FILE *fd, int i_inode) {
    /* 更新current_path */
    if (i_inode == 0) {
        // current_path[0] = '/';
        strcpy(current_path, "/");
    } else {
        for (int i = 0; i < 32; i++) {
            if (current_dir.inode[i] != 0 && i_inode == current_dir.inode[i]-1) {
                if (strcmp(current_dir.filename[i], ".") == 0) {
                    // cout << "stay in this dir" << endl;
                    break;
                }
                if (strcmp(current_dir.filename[i], "..") == 0) {
                    // cout << "back to parent dir" << endl;
                    char *tmp_name;
                    char tmp_name2[14];
                    tmp_name = strtok(current_path+1, "/");
                    while (tmp_name != NULL) {
                        strcpy(tmp_name2, tmp_name);
                        tmp_name = strtok(NULL, "/");
                    }
                    // cout << "tmp_name2 " << tmp_name2 << endl;
                    // cout << "current path " <<current_path << endl;
                    /*
                    int len = strlen(current_path) - strlen(tmp_name2) - 1;
                    cout << len << endl;
                    string str(current_path);
                    cout << "current_path " << current_path << endl;
                    // string str2 = str.substr(0, len);
                    strcpy(current_path, (str.substr(0, len)).c_str());
                    */
                    strcat(current_path, "/");
                    break;
                }
                if (current_path[strlen(current_path)-1] != '/')
                    strcat(current_path, "/");
                strcat(current_path, current_dir.filename[i]);
                strcat(current_path, "/");
                break;
            }
        }
        
    }

    /* 更新current_dir */
    current_dir.i_inode = i_inode;
    // clear current_dir
    for (int i = 0; i < 32; i++) {
        current_dir.inode[i] = 0;
        // current_dir.filename[i] = {0};
    }

    long offset;
    unsigned short tmp_inode;
    int index_file = 0;
    
    for (int z = 0; z < 9; z++) {
        if (inode[i_inode].i_zone[z] != 0) {
            offset = inode[i_inode].i_zone[z] * BLOCK_SIZE;
            fseek(fd, offset, SEEK_SET);
            for (int i = 0; i < 32; i++) {
                fread(&tmp_inode, 2, 1, fd);
                if (tmp_inode != 0) {
                    current_dir.inode[index_file] = tmp_inode;
                    fread(current_dir.filename[index_file++], 1, 14, fd);
                    fseek(fd, 16, SEEK_CUR);
                } else {
                    fseek(fd, 14 + 16, SEEK_CUR);
                }
            }
        }
    }
}

void show_current_dir() {
    cout << "current directory inode: " << current_dir.i_inode << endl;
    cout << setw(3) << "inode" << setw(15) << "file name" << setw(10) << "size" << endl;
    int count = 0;
    for (int i = 0; i < 32; i++) {
        if (current_dir.inode[i] != 0) {
            cout << setw(3) << current_dir.inode[i] << setw(15) << current_dir.filename[i] << setw(10) << inode[current_dir.inode[i]-1].i_size << endl; 
            count++;
        }
    }
    cout << "total count: " << count << endl;
}