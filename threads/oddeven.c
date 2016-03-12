#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int max=20;
int i = 0;

pthread_cond_t cond;// = PTHREAD_COND_INITAILIZER;
pthread_mutex_t mtx;// = PTHREAD_MUTEX_INITAILIZER;

void* oddprint()
{
    while(1) {
        pthread_mutex_lock(&mtx);

        if(i % 2 == 0 && i<=max) {
            pthread_cond_wait(&cond, &mtx);

            printf("ODD : %d\n", i);
            i++;
        }

        if(i > max) {
            pthread_mutex_unlock(&mtx);
            pthread_cond_signal(&cond);
            return NULL;
        }
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mtx);
    }
}

void* evenprint()
{
    while(1) {
        pthread_mutex_lock(&mtx);

        if(i % 2 == 1 && i <= max){
            pthread_cond_wait(&cond, &mtx);

            printf("EVEN : %d\n", i);
            i++;
        }

        if(i > max) {
            pthread_mutex_unlock(&mtx);
            pthread_cond_signal(&cond);
            return NULL;
        }

        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mtx);
    }
}

int main(int argc, char **argv)
{
    pthread_t oddthread, eventhread;

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&oddthread, NULL, oddprint, NULL);
    pthread_create(&eventhread, NULL, evenprint, NULL);

    pthread_join(oddthread, NULL);
    pthread_join(eventhread, NULL);

    return 0;
}
