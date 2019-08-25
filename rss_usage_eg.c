//This C code example demonstrates allocated memory going from virtual to resident (VSZ to RSS)
//From Ciro Sintilli's answer on StackOverflow August 11, 2019 : https://stackoverflow.com/questions/7880784/what-is-rss-and-vsz-in-linux-memory-management
//gcc -ggdb3 -O0 -std=c99 -Wall -Wextra -pedantic -o main.out main.c ./main.out
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
    char *base, *p;
    long page_size = sysconf(_SC_PAGESIZE);
    /* 16GiB + 1. */
    unsigned long malloc_size = (1ul << 34) + 1;
    unsigned long i = 0;
    base = (char*) malloc(malloc_size);
    if (base == NULL) {
        perror("malloc");
        exit(1);
    }
    p = base;
    while (p < base + malloc_size) {
        /* Modify a byte in the page. */
        *p = i;
        p += page_size;
        /* Print status every 4GiB. */
        if (i % (1ul << 32) == 0) {
            sleep(1);
            printf("%lu GiB\n", i / (1ul << 30));
            system("ps -ax -eo vsz,rss,comm | grep -E 'VSZ|main.out'");
            puts("");
        }
        i += page_size;
    }
    free(base);
    printf("i = 0x%lx\n", i);
    puts("done");
    return EXIT_SUCCESS;
}
