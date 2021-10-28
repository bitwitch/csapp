#include <unistd.h>

int main() {
    alarm(30);
    while(1) {}
    return 0;
}
