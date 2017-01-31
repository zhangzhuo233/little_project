#ifndef _UTILI_H
#define _UTILI_H

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<errno.h>
#include<limits.h>  //_POSIX_PATH_MAX在此头文件中定义,realpath()定义

typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define READMESIZE 1024
struct stat statbuf;

using namespace std;

#endif
