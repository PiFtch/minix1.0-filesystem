# minix1.0-filesystem
a simplified minix1.0-filesystem

简化版minix1.0文件系统
将磁盘的1024B划分为一个块BLOCK，对于一个磁盘，第0块为引导区，
第1块为超级块，接着数块inode节点位图，数块为磁盘块位图，
其后是存放inode的数个块，最后是可用于存放数据的块。

fs.h
包含磁盘上超级块结构定义和inode结构定义，以及相关常量和宏

main.cpp是一个演示程序，其在磁盘创建一个二进制文件（原始磁盘映像），
并且格式化为标准minix1.0文件系统，该磁盘映像可直接通过mount使用。
同时main.cpp包含了简单的命令解释功能

