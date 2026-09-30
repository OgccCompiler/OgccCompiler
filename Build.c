#include <stdio.h>
#include <stdlib.h>

int main() {
    #if defined(_WIN32) || defined(_WIN64)
        printf("[OGCC-Build] Execute commands...\n");
        if(system("where g++ > nul 2>&1") != 0) {
            system("pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain");
        }
    #elif defined(__apple__) || defined(__MACH__)
        printf("[OGCC-Build] Execute commands...\n");
        if(system("which g++ > /dev/null 2>&1") != 0) {
            system("brew install gcc");
        }
    #elif defined(__linux__)
        printf("[OGCC-Build] Execute commands...\n");
        if(system("which g++ > /dev/null 2>&1") != 0) {

        }
    #else
        printf("Unsupported operating system.\n");
    #endif
    return 0;
}