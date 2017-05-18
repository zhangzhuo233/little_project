#include"utili.h"


int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

/*overtime connect function,  options are ip_address, port and timeout/(ms),
 *Upon successful completion, unblock_connect() shall return socketfd on stat of connect;
 *otherwise, -1 shall be returned
 * */
int unblock_connect(const char* ip, int port, int time)
{
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int fdopt = setnonblocking(sockfd);
    ret = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
    if(ret == 0)
    {
        /*if successful connect, the attribute of sockfd are restore, otherwise, return immediately*/
        printf("connect with server immediately\n");
        fcntl(sockfd, F_SETFL, fdopt);
        return sockfd;
    }
    else if(errno != EINPROGRESS)
    {
        /*if connection is noe built, only when erron is EINPROGRESS, the connection is continued,
         * otherwise,return immediately
         * */
        printf("unblock connect not supprot\n");
        return -1;
    }

    fd_set readfds;
    fd_set writefds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(sockfd, &writefds);

    timeout.tv_sec = time;
    timeout.tv_usec = 0;

    ret = select(sockfd + 1, NULL, &writefds, NULL, &timeout);
    if(ret <= 0)
    {
        /*select is over time or error, return immediately*/
         printf("connection time out\n");
         close(sockfd);
         return -1;
     }

    int error = 0;
    socklen_t length = sizeof(error);
    /*use getsockopt() to get and clear the error of sockfd*/
    if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &length) < 0)
    {
         printf("get socket option failed\n");
         close(sockfd);
         return -1;
    }
    /*error is not 0, to express connection is error*/
    if(error != 0)
    {
         printf("connection failed after select with the error %d \n", error);
         close(sockfd);
         return -1;
    }
    /*connect success*/
    printf("connection ready after select with the socket: %d \n", sockfd);
    fcntl(sockfd, F_SETFL, fdopt);
    return sockfd;
}

int main(int argc, char const* argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd = unblock_connect(ip, port, 0);
    if(sockfd < 0)
    {
        return 1;
    }
    return 0;
}
