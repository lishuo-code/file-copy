#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int src, dst;
    struct stat sb;
    int *addr_src, *addr_dst;
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
    
    dst = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dst < 0){
        close(src);
        printf("create dst file failed!\n");
        return -1;
    }

    fstat(src, &sb);
    addr_src = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, src, 0);
    if(addr_src==MAP_FAILED)
        printf("addr_src mmap failed, errmsg=%s\n", strerror(errno));
    close(src);
    ftruncate(dst, sb.st_size);
    addr_dst = mmap(NULL, sb.st_size, PROT_WRITE, MAP_SHARED, dst, 0);
    if(addr_dst==MAP_FAILED)
        printf("addr_dst mmap failed, errmsg=%s\n", strerror(errno));
    close(dst);
    
    memcpy(addr_dst, addr_src, sb.st_size);
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf( "Copy program of open,mmap takes %lf seconds\n", duration );
    return 0;
}
