#include"utili.h"


int main(int argc, char const* argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address portnumber\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    /* create TCP sockfd, and bind with port*/
    int listenfd = startup(ip, port);

    /* create UDP sockfd, and bind with port*/
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(udpfd >= 0);

    ret = bind(udpfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);

    /*register readable events on TCP socket and UDP socket*/
    addfd(epollfd, listenfd);
    addfd(epollfd, udpfd);

    while(1)
    {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if(number < 0)
        {
            printf("epoll failure\n");
            break;
        }

        for(int i = 0; i < number;  ++i)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd)
            {
                 struct sockaddr_in client_address;
                 socklen_t client_addrlen = sizeof(client_address);
                 int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlen);
                 addfd(epollfd, connfd);
            }
            else if(sockfd == udpfd)
            {
                char buf[UDP_BUFFER_SIZE];
                memset(buf, '\0', UDP_BUFFER_SIZE);
                struct sockaddr_in client_address;
                socklen_t client_addrlen = sizeof(client_address);

                ret = recvfrom(udpfd, buf, UDP_BUFFER_SIZE - 1, 0, (struct sockaddr*)&client_address, &client_addrlen);

                if(ret > 0)
                {
                     sendto(udpfd, buf, UDP_BUFFER_SIZE - 1, 0, (struct sockaddr*)&client_address, client_addrlen);
                }
            }
            else if(events[i].events & EPOLLIN)
            {
                char buf[TCP_BUFFER_SIZE];
                while(1)
                {
                    memset(buf, '\0', TCP_BUFFER_SIZE);
                    ret = recv(sockfd, buf, TCP_BUFFER_SIZE - 1, 0);
                    if(ret < 0)
                    {
                        if((errno == EAGAIN) || (errno == EWOULDBLOCK))
                        {
                            break;
                        }
                        close(sockfd);
                        break;
                    }
                    else if(ret == 0)
                    {
                         close(sockfd);
                    }
                    else
                    {
                        send(sockfd, buf, ret, 0);
                    }
                }
            }
            else
            {
                 printf("something else happened\n");
            }
        }
    }

    close(listenfd);
    return 0;
}
