#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h> 
#include <stdlib.h> 


void* target(void *arg)
{
    char name[20];
    int val;

    val = *(int *)arg;

//    printf("val : %d \n", val);

    sleep(1);

    if ( pthread_getname_np(pthread_self(), name, 20) != 0) {
        fprintf(stderr, "Error while fetching the name of : %ld\n", pthread_self());
	pthread_exit(NULL);
    }
    
    printf("I am thread: %d and my name is: %s \n",val, name);
}


int main(int argc, char **argv)
{
    pthread_t tid;
    pthread_t tids[5];
    int status, i;
    char *tname[] = {"one", "two", "three", "four", "five"};

    for(i=0; i<5; i++) {
        // To create multiple threads from this loop. All threads
	// target is the same function.
        status = pthread_create(&tids[i], NULL, target, (void *)&i);

	#if 0
        if(status == -1) {
            perror("failure in thread creation");
	    exit(EXIT_FAILURE);
        }
	#endif

	//puts(tname[i]);
	//printf("id : %ld\n", tids[i]);

	if (pthread_setname_np(tids[i], tname[i]) != 0) {
	    perror("Thread setname failure... : ");
	    exit(EXIT_FAILURE);
        }

	printf("Thread %d created succesfully...\n", i+1);
	//pthread_join(tids[i], NULL);
    }

#if 0
    status = pthread_create(&tid, NULL, target, NULL);
    if(status == -1) {
        perror("failure in thread creation");
	exit(EXIT_FAILURE);
    }
    
    printf("In Main\n");
#endif
    
    for(i=0; i<5; i++) {
	pthread_join(tids[i], NULL);
    }


    return 0;
}
