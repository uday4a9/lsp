#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 


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
	puts("Connection established as : %d", a);
        printf("Request from ip:%s\n",inet_ntop(AF_INET,&cli.sin_addr,str,sizeof(str)));

    }

    return 0;
}
