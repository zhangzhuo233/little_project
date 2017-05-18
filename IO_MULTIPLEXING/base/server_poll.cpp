#include"utili.h"
#include<poll.h>

#define MAX_CLIENT_SIZE 5

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

    int i = 0;
    char buffer[256];

    struct pollfd client_fd[MAX_CLIENT_SIZE];

    client_fd[0].fd = serverfd;
    client_fd[0].events = POLLIN;
    client_fd[0].revents = 0;

    for(i = 1; i < MAX_CLIENT_SIZE + 1; ++i)
    {
        client_fd[i].fd = 0;
    }

    nfds_t nfds = 0;
    int ret = 0;
    while(1)
    {
        ret = poll(client_fd, nfds + 1, -1);
        if(ret == -1)
        {
            perror("poll ");
            continue;
        }
        else
        {
            if(client_fd[0].revents & POLLIN)
            {
                struct sockaddr_in address;
                socklen_t len = sizeof(address);
                int sockconn = accept(serverfd, (struct sockaddr*)&address, &len);
                if(sockconn == -1)
                {
                    perror("accept ");
                    continue;
                }
                for(i = 1; i < MAX_CLIENT_SIZE + 1; ++i)
                {
                    if(client_fd[i].fd == 0)
                    {
                        client_fd[i].fd = sockconn;
                        client_fd[i].events = POLLIN;
                        nfds++;
                        break;
                    }
                }
                if(i == MAX_CLIENT_SIZE)
                {
                    const char* msg = "too many client\n";
                    send(sockconn, msg, strlen(msg) + 1, 0);
                }
                continue;
            }
            for(i = 1; i < MAX_CLIENT_SIZE + 1; ++i)
            {
                if(client_fd[i].revents & POLLIN)
                {
                    recv(client_fd[i].fd, buffer, 256, 0);
                    printf("client msg:>%s\n", buffer);
                    send(client_fd[i].fd, buffer, strlen(buffer) + 1, 0);
                }
                else if(client_fd[i].revents & POLLRDHUP)
                {
                     close(client_fd[i].fd);
                     client_fd[i] = client_fd[nfds];
                     nfds--;
                     printf("a client left\n");
                }
            }
        }
    }
    close(serverfd);
    return 0;
}
