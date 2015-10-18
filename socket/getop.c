#include <stdio.h>
#include <assert.h>
#include <getopt.h>

int main(int argc, char **argv)
{
    int i;

    for(i=1; i<argc; i++) {
        puts(argv[i]);
    }
    return 0;
}
