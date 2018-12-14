#ifndef FS_H
#define FS_H

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <iomanip>
#include <bitset>
#include <cmath>
#include <ctime>

#include "../include/buffer.h"
using namespace std;
#pragma pack(1)

struct d_super_block {
	unsigned short s_ninodes;
	unsigned short s_nzones;
	unsigned short s_imap_blocks;
	unsigned short s_zmap_blocks;
	unsigned short s_firstdatazone;
	unsigned short s_log_zone_size;
	unsigned int s_max_size;
	unsigned short s_magic;
};
// d_super_block super_block;

struct d_inode {
	unsigned short i_mode;
	unsigned short i_uid;
	unsigned int i_size;
	unsigned int i_time;
	unsigned char i_gid;
	unsigned char i_nlinks;
	unsigned short i_zone[9];
};

#define FILESYSTEM "/mnt/Workspace/minix/filesystem.img"

#define DISK_SIZE
#define INODE_COUNT 32
#define BLOCK_COUNT 36
#define BLOCK_SIZE 1024

#define INODE_BITMAP_COUNT 1
#define BLOCK_BITMAP_COUNT 1
#define D_SUPER_BLOCK_COUNT 1
#define INODE_BLOCK_COUNT 1


#define D_SUPER_BLOCK_START 1*BLOCK_SIZE
#define INODE_BITMAP_START 2*BLOCK_SIZE
#define BLOCK_BITMAP_START 3*BLOCK_SIZE
#define INODE_START 4*BLOCK_SIZE
#define DATA_START 5*BLOCK_SIZE

#define SIZE_OF_D_SUPER_BLOCK sizeof(d_super_block)
#define SIZE_OF_D_INODE sizeof(d_inode)
// d_inode inode[INODE_COUNT];

// unsigned char inode_bitmap[INODE_BITMAP_COUNT*BLOCK_SIZE/8];

// struct {
// 	int i_inode;
// 	int inode[32];
// 	char filename[32][14];
// }current_dir;
struct dir {
	int i_inode;
	int inode[32];
	char filename[32][14];
};

#endif