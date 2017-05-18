#include"utili.h"

int main(int argc, char* argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return -1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int sockCli = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrSer, addrCli;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(port);
    addrSer.sin_addr.s_addr = inet_addr(ip);

    socklen_t len = sizeof(struct sockaddr);
    int ret = connect(sockCli, (struct sockaddr*)&addrSer, len);
    assert(ret != -1);

    char sendbuf[256];
    char recvbuf[256];
    while(1)
    {
        printf("msg:>");
        scanf("%s",sendbuf);
        send(sockCli, sendbuf, strlen(sendbuf)+1, 0);
        recv(sockCli, recvbuf, 256, 0);
        printf("From myself msg:> %s\n",recvbuf);
    }
    close(sockCli);
    return 0;
}
