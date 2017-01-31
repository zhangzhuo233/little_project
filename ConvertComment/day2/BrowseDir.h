#ifndef _BROWSEDIR_H
#define _BROWSEDIR_H

#include"utili.h"
#include"ConvertComment.h"

void FileCp(char *des, char *src);              //复制文件
BOOL IsCFile(char *file);                       //判断是否是.c文件
BOOL IsCppFile(char *file);                     //判断是否是.cpp文件
char* GetFileName(char *NamePath);              //只获取文件名or目录名
void PathCat(char *desPath, char *srcPath);     //目录后追加文件名或者路径
int PathCmp(char *dir1, char *dir2);            //(注释转换没用到)比较两个目录是否同级（深度相同）1 0 -1
int PathDep(char *dir);                         //(注释转换没用到)统计目录的深度
void help();                                    //帮助文档
void SeeArgc(const int count);                  //判断参数数量是否正确
void OneFileCovert(char *des, char *src);       //单个文件的注释转换
void DirConvert(char *des, char *src);          //目录的递归转换
void BeginConvert(char *des, char *src);        //进行目录递归转换
size_t GetFileSize(FILE *NamePath);             //获取一个文件的大小

size_t GetFileSize(FILE *NamePath)
{
    size_t size = 0;
    fseek(NamePath, 0, SEEK_END);
    size = ftell(NamePath);
    cout<<"大小为："<<size<<endl;
    return size;
}
void FileCp(char *des, char *src)
{
	FILE *inputfile = fopen(src, "rb");
	if(NULL == inputfile)
	{
		cout<<"Open src file Fail!"<<endl;
        exit(EXIT_FAILURE);
	}
    size_t SIZE = GetFileSize(inputfile);
    fseek(inputfile, 0, SEEK_SET);          //使文件流指针回到文件开始
    char buf[SIZE+1];

    FILE *outputfile = fopen(des, "wb");
	if(NULL == outputfile)
	{
		cout<<"Open des file Fail!"<<endl;
        exit(EXIT_FAILURE);
	}
    while(!feof(inputfile))
    {
        fread(buf, sizeof(char), SIZE, inputfile);
        fwrite(buf, sizeof(char), SIZE, outputfile);
    }
	fclose(inputfile);
	fclose(outputfile);
}
BOOL IsCFile(char *file)
{
    int len = strlen(file);
    if(file[len-1] == 'c' && file[len-2] == '.')
        return TRUE;
    else
        return FALSE;
}
BOOL IsCppFile(char *file)
{
    int len = strlen(file);
    if(file[len-1] == 'p' && file[len-2] == 'p'
        && file[len-3] == 'c' && file[len-4] == '.')
        return TRUE;
    else
        return FALSE;
}
char* GetFileName(char *NamePath)
{
    if(NULL == NamePath)
    {
        cout<<"NamePath is NULL!"<<endl;
        exit(EXIT_FAILURE);
    }
    int len = strlen(NamePath);
    char *Current = NULL;
    while(len > 0 && NamePath[len] != '/')
    {
         len--;
    }
    if(NamePath[len] == '/')
        Current = &NamePath[len] + 1;
    else
        Current = &NamePath[len];
    return Current;
}
void PathCat(char *desPath, char *srcPath)
{
    char          *ptrdes;
    char *srcPathname = GetFileName(srcPath);
    if(lstat(desPath, &statbuf) < 0)
    {
        cout<<"lstat error!"<<endl;
        exit(EXIT_FAILURE);
    }
    if(desPath[strlen(desPath)] != '/' && S_ISDIR(statbuf.st_mode) != 0)
    {
         ptrdes = desPath + strlen(desPath);
         *ptrdes++ = '/';
         *ptrdes = 0;
    }
    strcat(desPath, srcPathname);
}
int PathDep(char *dir)
{
    int len = strlen(dir);
    int count = 0;
    while(len-- > 0)
    {
         if(dir[len] == '/')
             ++count;
    }
    return count;
}
int PathCmp(char *dir1, char *dir2)
{
    if(PathDep(dir1) > PathDep(dir2))
        return 1;
    else if(PathDep(dir1) == PathDep(dir2))
        return 0;
    else
        return -1;
}

void OneFileCovert(char *des, char *src)
{
	FILE *inputfile = fopen(src, "r");
	if(NULL == inputfile)
	{
		cout<<"Open src file Fail!"<<endl;
        exit(EXIT_FAILURE);
	}
	FILE *outputfile = fopen(des, "w");
	if(NULL == outputfile)
	{
		cout<<"Open des file Fail!"<<endl;
        exit(EXIT_FAILURE);
	}

	ConvertComment(inputfile, outputfile);

	fclose(inputfile);
	fclose(outputfile);
}
void DirConvert(char *des, char *src)
{
    char src_path[_POSIX_PATH_MAX];
    memset(src_path, 0, _POSIX_PATH_MAX);

    struct dirent *dirp;
    DIR           *dpsrc;               //源目录
    DIR           *dpdes;               //目的目录
    if(NULL == des || NULL == src)      //参数检测
    {
        cout<<"des/src is NULL!"<<endl;
        exit(EXIT_FAILURE);
    }
    if(NULL == (dpdes = opendir(des)))  //如果目的目录不存在，则创建
    {
        mkdir(des, 0755);
    }
    if(NULL == (dpsrc = opendir(src)))  //如果源目录不存在，退出程序
    {
         perror("opendir src");
         exit(EXIT_FAILURE);
    }
    if(NULL == realpath(src,src_path))  //src给src_path
    {
        perror("realpath src");
        exit(EXIT_FAILURE);
    }
    //readdir读取src的顺序为:t1,t2,t3,input.c
    while((dirp = readdir(dpsrc)) != NULL)
    {
        if(strcmp(dirp->d_name, ".") == 0 ||
           strcmp(dirp->d_name, "..") == 0)
            continue;                   //忽略"."和".."
        PathCat(src_path,dirp->d_name);
        cout<<src_path<<endl;
        BeginConvert(des,src_path);   //des=./t1 src_path=./tttttt/t1
        if(NULL == realpath(src,src_path))//将src_path恢复成src
        {
            perror("realpath src");
            exit(EXIT_FAILURE);
        }
    }
    closedir(dpsrc);
    closedir(dpdes);
}
void BeginConvert(char *des, char *src) //des=./t1  src=./tttttt/t1
{
    //(1)将src,des的路径转化成绝对路径分别存于src_path,des_path中
    //(2)获取src的属性。
    //(3)分情况讨论
    //a.如果src是文件,判断是否是.c/.cpp文件，des_path追加这一文件，然后直接调动OneFileConvert()进行转换
    //b.如果src是目录,des_path追加这一目录，然后DirpConvert(des_path,src_path)

    //(1)
    char *srcfilename = GetFileName(src);
    char src_path[_POSIX_PATH_MAX];
    memset(src_path, 0, _POSIX_PATH_MAX);
    char des_path[_POSIX_PATH_MAX];
    memset(des_path, 0, _POSIX_PATH_MAX);
    if(NULL == realpath(src, src_path))
    {
        perror("realpath src");
        exit(EXIT_FAILURE);
    }
    if(NULL == realpath(des, des_path))
    {
        perror("realpath des");
        exit(EXIT_FAILURE);
    }
    //(2)
    if(lstat(src_path, &statbuf) < 0)
    {
         perror("lstat src");
         exit(EXIT_FAILURE);
    }
    //(3)
    //a.文件
    if(S_ISDIR(statbuf.st_mode) == 0)
    {
        if(IsCFile(src_path) || IsCppFile(src_path))
        {
            PathCat(des_path, srcfilename);
            OneFileCovert(des_path, src_path);
        }
        else    //只是单纯的写拷贝文件
        {
            PathCat(des_path, srcfilename);
            FileCp(des_path, src_path);
        }
    }
    //b.目录
    else
    {
        PathCat(des_path, srcfilename);
        DirConvert(des_path, src_path);
    }
}
void help()
{
     FILE *README = fopen("README.md", "r");
     if(NULL == README)
     {
          cout<<"Open README.md fail!"<<endl;
          exit(EXIT_FAILURE);
     }
     char buf[READMESIZE];
     while(!feof(README))
     {
          fgets(buf, READMESIZE, README);
          cout<<buf<<endl;
     }
}
void SeeArgc(const int count)
{
    if(count != 4)
    {
        cout<<"Argument is error!"<<endl;
        exit(EXIT_FAILURE);
    }
}

#endif
