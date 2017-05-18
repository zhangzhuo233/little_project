#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <libgen.h>
#define LISTEN_QUEUE_SIZE 5
#define MAX_BUFFER_SIZE 256

int startup(const char* ip, unsigned short port)
{
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    socklen_t len = sizeof(address);

    //int yes = 1;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    //setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    ret = bind(listenfd, (struct sockaddr*)&address, len);
    assert(ret != -1);
    ret = listen(listenfd, LISTEN_QUEUE_SIZE);
    assert(ret != -1);

    return listenfd;
}

