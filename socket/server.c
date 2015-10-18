#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <pthread.h>
#define _GNU_SOURCE
#include <string.h>
#include <unistd.h>
#include <libgen.h>

// pass multiple set of data

char *serverlogs = "/home/bujji/serverlogs";
struct udef{
    int sock;
};

char* chk_exist_n_create(char *name)
{
    int a;
    static char clientdir[256];
    
    memset(clientdir, 0, 256);
    strcat(clientdir, serverlogs);
    strcat(clientdir, "/"); 
    strcat(clientdir, name); 

    a = access(clientdir, F_OK);
    if (a == -1) {
        printf("New client, Create a specifc dir : %s\n", clientdir);
        if ( mkdir(clientdir, 0777) < 0) {
	    perror("MKDIR");
	    return NULL;
        }
    }
    return clientdir;
}

void check_n_copy(char *name, char *file)
{
    char buff[1024], path[512], *np, cpcmd[512];
    int tfd, efd, rd, wr;

    memset(path, 0, 512);
    np = chk_exist_n_create(name);
    if(np == NULL) {
        perror("chk_exist_n_create");
        return;
    }
    strcpy(path, np);
    strcat(path, "/");
    strcat(path, basename(file));
    
    printf ("performing : cp %s \n", file);//, path);
    sprintf(cpcmd, "cp %s %s", file, path);
    puts(cpcmd);
    system(cpcmd);
}

void* communicate(void *arg)
{
    int sock, r;
    char file[156], name[156];

    sock = ((struct udef *) (arg))->sock;
    r = recv(sock, name, sizeof(name), 0);
    if( r < 0) {
        perror("Client Name : READ");
        pthread_exit(NULL);
    }
    name[r] = '\0';
    printf("Client Name : %s\n", name);

    r = recv(sock, file, sizeof(file), 0);
    if( r < 0) {
        perror("File Name : READ");
        pthread_exit(NULL);
    }
    file[r] = '\0';
    printf("File Name : %s\n", file);

    check_n_copy(name, file);
    close(r);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int s, b, l, a, size;
    char ip[100];
    struct sockaddr_in ser, cli;

    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("SOCKET CREATION FAILED");
	return -1;
    }
    puts("Socket created succesfully");

    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(5000);
    ser.sin_addr.s_addr = INADDR_ANY;
 
    if((b = bind(s, (struct sockaddr *) &ser, sizeof(ser)) ) < 0) {
        perror("BINDING FAILED");
	return -1;
    }
    puts("Binded succesfully");

    if( (l = listen(s, 5)) < 0) {
        perror("LISTENING FAILED");
	return -1;
    }
    puts("Listening state succesfuly added");

    size = sizeof(cli);
    char str[100], fil[30];
    while(1) {
        puts("Waiting for a connection to be happened");
        if ((a = accept(s, (struct sockaddr *)(&cli), &size)) < 0) {
            perror("ACCEPT FAILURE");
	    return -1;
        }
	printf("Connection established as : %d", a);
        printf("Request from ip:%s\n",inet_ntop(AF_INET,&cli.sin_addr,str,sizeof(str)));

	// Create a seperate connnection for each client
	// for supporting concurrent server application
	pthread_t tid;
	struct udef sd = {a};

	pthread_create(&tid, NULL, communicate, (void*) &sd);

    }

    return 0;
}
