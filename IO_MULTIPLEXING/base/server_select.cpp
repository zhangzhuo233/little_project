/*select and out-of-band，OOB
 * */
#include <sys/select.h>
#include <vector>
#include <algorithm>
#include "utili.h"

using namespace std;

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

    char buffer[256] = {0};
    socklen_t len = sizeof(struct sockaddr);
    int numconn = 0;
    int max_sock = serverfd;
    int second_sock = serverfd;
    int ret = 0;
    char buf[1024];
    fd_set readfds;
    fd_set exceptfds;
    int client_fd[MAX_CLIENT_SIZE] = {0};
    struct timeval tv;
    while(1)
    {
        //1 accept
        //2 recv send
        FD_ZERO(&readfds);
        FD_ZERO(&exceptfds);
        FD_SET(serverfd, &readfds);
        FD_SET(serverfd, &exceptfds);

        for(int i=0; i<MAX_CLIENT_SIZE; ++i)
        {
            if(client_fd[i] != 0)
            {
                FD_SET(client_fd[i], &readfds);
            }
        }

        tv.tv_sec = 1000;
        tv.tv_usec = 0;

        //ret = select(max_sock + 1, &readfds, NULL, &exceptfds, &tv);
        ret = select(max_sock + 1, &readfds, NULL, &exceptfds, NULL);
        if(ret < 0)
        {
            printf("selection failure\n");
            continue;
        }
        else if(ret == 0)
        {
            printf("server time out\n");
            continue;
        }
        else
        {
            /*if the condition is read events, use recv() to read data*/
            if(FD_ISSET(serverfd, &readfds))
            {
                printf("hhhhhhhhhhhh\n");
                struct sockaddr_in clientaddress;
                int connfd = accept(serverfd, (struct sockaddr*)&clientaddress, &len);
                assert(connfd != -1);
                if(numconn < MAX_CLIENT_SIZE)
                {
                    client_fd[numconn++] = connfd;
                    if(connfd > max_sock)
                    {
                        int temp = max_sock;
                        max_sock = connfd;
                        second_sock = temp;
                    }
                }
                else
                {
                    const char *msg = "sorry, server overload";
                    send(connfd, msg, strlen(msg) + 1, 0);
                }
                continue;
            }

            for(int i=0; i<numconn; ++i)
            {
                if(FD_ISSET(client_fd[i], &readfds))
                {
                    ret = recv(client_fd[i], buffer, 256, 0);
                    if(ret <= 0)
                    {
                        printf("client leveal.\n");
                        close(client_fd[i]);
                        if(client_fd[i] == max_sock)
                        {
                            max_sock = second_sock;
                        }
                        numconn--;
                        break;
                    }
                    //else if(ret == 0)
                    //{
                    //    printf("no pi\n");
                    //    break;
                    //}
                    printf("client%d msg:>%s\n", i, buffer);
                    send(client_fd[i], buffer, strlen(buffer)+1, 0);
                }
                else if(FD_ISSET(client_fd[i], &exceptfds))
                {
                    ret = recv(client_fd[i], buffer, sizeof(buffer) - 1, MSG_OOB);
                    if(ret <= 0)
                        break;
                    printf("get %d bytes of oob data:%s\n", ret , buffer);
                }
            }
        }
    }

    return 0;
}
