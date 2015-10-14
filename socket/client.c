#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

int main(int argc, char **argv)
{
    int s, c,  size;
    struct sockaddr_in ser, cli;

    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("SOCKET CREATION FAILED");
	return -1;
    }
    puts("Socket created succesfully");

    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(5000);
    ser.sin_addr.s_addr = htonl(INADDR_ANY);
 
    if ((c = connect(s, (struct sockaddr *) &ser, sizeof(ser))) < 0) {
        perror("CONNECT FAILED");
	return -1;
    }
    puts("Connection established succesfully\n");
    
    return 0;
}
