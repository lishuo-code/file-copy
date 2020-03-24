#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int src, dst;
    int len = 0;
    char buff[1025] = {0};
    clock_t start, end;
    double duration;
    start = clock();
    if(argc != 3){
        printf("command line arguments error!\n");
        return -1;
    }
    src = open(argv[1], O_RDONLY);
    if (src < 0){
        printf("the src file not exist!\n");
        return -1;
    }
    
    dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (dst < 0){
        close(src);
        printf("create dst file failed!\n");
        return -1;
    }
    
    while((len = read(src, buff, 1024)) > 0){
        write(dst, buff, len);
    }
    
    close(src);
    close(dst);
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Copy program of open,read,write takes %lf seconds\n", duration );
    return 0;
}



