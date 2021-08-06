/* Problem 8.20 */

#include <unistd.h>

int main(int argc, char** argv, char** envp) {
    if(execve("/bin/ls", argv, envp) == -1) {
        return 1;
    }
}

