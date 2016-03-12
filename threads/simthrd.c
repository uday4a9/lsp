#include <stdio.h>
#include <assert.h>
#include <pthread.h>

void* fun()
{
    pthread_exit(NULL);
    puts("FUN");
}

int main(int argc, char **argv)
{
    pthread_t tid;

    pthread_create(&tid, NULL, fun, NULL);

    puts("IN MAIN");
    pthread_join(tid, NULL);

    return 0;
}
