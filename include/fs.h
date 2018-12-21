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

// #include "../include/buffer.h"
using namespace std;
#pragma pack(1)

struct d_super_block {
	unsigned short s_ninodes;	// inode数
	unsigned short s_nzones;	// block数
	unsigned short s_imap_blocks;	// inode位图块数
	unsigned short s_zmap_blocks;	// block位图块数
	unsigned short s_firstdatazone;	// 首个数据块
	unsigned short s_log_zone_size;	// 日志
	unsigned int s_max_size;	// 允许的最大大小
	unsigned short s_magic;		// 魔数
};

struct d_inode {
	unsigned short i_mode;	// 模式
	unsigned short i_uid;	// 用户ID
	unsigned int i_size;	// 大小
	unsigned int i_time;	// 修改时间
	unsigned char i_gid;	// 组ID
	unsigned char i_nlinks;	// 链接数
	unsigned short i_zone[9];	// 文件所用块，取消了三级索引，全为直接索引
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

#define AVAILABLE_BLOCK_COUNT BLOCK_COUNT-1-1-INODE_BITMAP_COUNT-BLOCK_BITMAP_COUNT-INODE_BLOCK_COUNT	// 可用块

#define SIZE_OF_D_SUPER_BLOCK sizeof(d_super_block)
#define SIZE_OF_D_INODE sizeof(d_inode)

#define I_MODE_DIR 16877
#define I_MODE_NORMAL 33188
#define I_UID 1000

/* 目录结构 */
struct dir {
	int i_inode;
	int inode[32];
	char filename[32][14];
	unsigned char block[32][32];
};

#endif