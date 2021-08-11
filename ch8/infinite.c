#include <unistd.h>

int main() {
    alarm(60);
    while(1) {}
    return 0;
}
