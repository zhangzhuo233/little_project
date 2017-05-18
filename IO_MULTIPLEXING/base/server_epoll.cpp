#include<sys/epoll.h>
#include"utili.h"

#define MAX_EVENT_SIZE 1024

void add_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

int main(int argc, char* argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return -1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int serverfd = startup(ip, port);
    int epollfd = epoll_create(5);

    char buffer[256] = {0};
    add_event(epollfd, serverfd, EPOLLIN);
    int ret = 0;
    struct epoll_event events[MAX_EVENT_SIZE];
    int i;
    while(1)
    {
         ret = epoll_wait(epollfd, events, MAX_EVENT_SIZE, -1);
         if(ret == -1)
         {
             perror("epoll_wait ");
             continue;
         }
         else
         {
              for(i = 0; i < ret; ++i)
              {
                  int fd = events[i].data.fd;
                  if((fd == serverfd) && (events[i].events & EPOLLIN))
                  {
                       struct sockaddr_in address;
                       socklen_t len = sizeof(address);
                       int sockconn = accept(serverfd, (struct sockaddr*)&address, &len);
                       if(sockconn == -1)
                       {
                            perror("accept ");
                            break;
                       }
                       add_event(epollfd, sockconn, EPOLLIN);
                  }
                  else if(events[i].events & EPOLLIN)
                  {
                      recv(events[i].data.fd, buffer, 256, 0);
                      printf("client msg:>%s\n", buffer);
                      send(events[i].data.fd, buffer, strlen(buffer) + 1, 0);
                  }
              }
         }
    }

    close(serverfd);
    return 0;
}
