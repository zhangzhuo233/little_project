#ifndef __UTILI_H__
#define __UTILI_H__

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024
#define LISTEN_QUEUE_SIZE 5

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd)
{
     epoll_event event;
     event.data.fd = fd;
     event.events = EPOLLIN | EPOLLET;
     epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
     setnonblocking(fd);
}

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
#endif
