#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

#include <assert.h>

int main()
{
    int sock, b, l, rc, len;
    struct sockaddr_in myaddr, peeraddr;
    struct sockaddr_in addr;
    socklen_t slen;

    if((sock = socket(PF_INET, SOCK_STREAM, 0) )== -1) {
        perror("SOCKET");
        return -1;
    }
    printf("socket created : %d\n", sock);

    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = PF_INET;
    myaddr.sin_port = htons(80);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    if( (b = bind(sock, (struct sockaddr *) &myaddr, sizeof(myaddr))) == -1) {
        perror("BIND");
        return -1;
    }
    puts("Binded succesfully");

    if((l = listen(sock, 2)) == -1){
        perror("LISTEN");
        return -1;
    } 
    puts("Server is listening");

    slen = sizeof(addr);

    if(getsockname(sock, (struct sockaddr *)&addr, &slen) == -1) {
        perror("GETSOCKNAME");
        return -1;
    }

    printf("server available at (%s, %d)\n",inet_ntoa(addr.sin_addr), (int)ntohs(addr.sin_port));

    
    close(sock);

    return 0;
}
