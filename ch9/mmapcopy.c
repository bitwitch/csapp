#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

// Practice Problem 9.5
int main(int argc, char **argv) {
    int fd = open(argv[1], O_RDONLY);
    size_t file_size_in_bytes = lseek(fd, 0L, SEEK_END);
    off_t offset = 0;
    void *file_data = mmap(NULL, file_size_in_bytes, PROT_READ, MAP_PRIVATE, fd, offset);
    close(fd);
    write(STDOUT_FILENO, file_data, file_size_in_bytes);
    return 0;
}

