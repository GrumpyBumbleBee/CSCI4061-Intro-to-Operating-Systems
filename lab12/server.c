#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 5
#define STRSZ 100
#define PORT 3000 

int main(int argc, char *argv[]) {
    int listen_fd, conn_fd;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
        perror("socket error");

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    int ret = bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if(ret == -1)
        perror("bind error");

    ret = listen(listen_fd, BACKLOG);
    if(ret == -1)
        perror("listen error");

    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    conn_fd = accept(listen_fd, (struct sockaddr *) &clientaddr, &clientaddr_len);
    if(conn_fd == -1)
        perror("accept error");

    char msg[STRSZ];
    while(1) {
        bzero(msg, STRSZ);
        ret = recv(conn_fd, msg, STRSZ, 0);
        if(ret == -1)
            perror("recv error");
        fprintf(stdout, "Client: %s\n", msg);

        if(strcmp(msg, "END") == 0)
            break;

        bzero(msg, STRSZ);
        fprintf(stdout, "Server: ");
        fscanf(stdin, "%[^\n]s", msg);
        setbuf(stdin, NULL);
        ret = send(conn_fd, msg, STRSZ, 0);
        if(ret == -1)
            perror("send error");
    }
    close(conn_fd);
    close(listen_fd);
    fprintf(stdout, "Server exiting...\n");
    return 0;

}