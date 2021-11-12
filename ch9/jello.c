#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>



int main() {
    int fd = open("hello.txt", O_RDWR);

    off_t file_size_in_bytes = lseek(fd, 0L, SEEK_END);

    char *memory = (char *)mmap(NULL, file_size_in_bytes, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    close(fd);

    if (!memory) {
        printf("mmap failed.\n");
        return 1;
    }

    memory[0] = 'J';

    return 0;
}
