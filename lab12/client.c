#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define STRSZ 100
#define PORT 3000


int main(int argc, char *argv[]) {
    /*
    ---------- Note ----------
    IMPORTANT!

    Do not copy the code directly from server.c. Some of them are NOT applicable to the client.c

    Please use slides to help you fill out the blank

    */
   
    // 1st arg of socket is the address family. Good examples are IPv4 and IPv6
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
        perror("socket error");

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET; // What is the address family?
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // What is the server's address? Hint: what is IPv4 of localhost?
    servaddr.sin_port = htons(PORT); // 1. What is the server's port number? 2. Convert the host's little endian to network's big endian
    
    int ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(ret == -1)
        perror("connect error");

    char msg[STRSZ];
    while(1) {
        bzero(msg, STRSZ);
        fprintf(stdout, "Client: ");
        fscanf(stdin, "%[^\n]s", msg);
        setbuf(stdin, NULL);
        ret = send(sockfd, msg, STRSZ, 0); // TODO: Fill in the blank
        if(ret == -1)
            perror("send error");

        if(strcmp(msg, "END") == 0)
            break;

        bzero(msg, STRSZ);
        ret = recv(sockfd, msg, STRSZ, 0); // TODO: Fill in the blank
        if(ret == -1)
            perror("recv error");
        fprintf(stdout, "Server: %s\n", msg);
    }
    close(sockfd);
    fprintf(stdout, "Client exiting...\n");
    return 0;
}