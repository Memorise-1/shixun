/*===============================================
*   文件名称：2.c
*   创 建 者：     
*   创建日期：2026年04月14日
*   描    述：
================================================*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char *argv[])
{ 
int fd;
fd=open("1.txt",O_RDWR|O_CREAT,0664);
if(-1==fd){
    perror("open fail\n");
    return -1;
} 
printf("fd is%d\n",fd);
size_t ret;
char buf[1024]={0};
ret=read(fd,buf,8);
if(ret==-1){
    perror("read fall\n");
    return -1;
}
printf("%s\n",buf);
char buf1[64]="曼波";
size_t ret1;
ret1=write(fd,buf1,64);
if(ret==-1){
    perror("write fail\n");
    return -1;
}
close(fd);
return 0;
}
