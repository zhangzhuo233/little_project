#include"utili.h"

void cli_tcp_communic(char *ip, int port)
{
    printf("This is TDP client...\n");
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
}
void cli_udp_communic(char *ip, int port)
{
    printf("This is UDP client...\n");
    //socket
    int sockCli = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockCli == -1)
    {
        perror("socket Cli");
        exit(1);
    }
    struct sockaddr_in addrSer;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(port);
    addrSer.sin_addr.s_addr = inet_addr(ip);
    socklen_t len = sizeof(struct sockaddr);

    char *msg = (char*)malloc(sizeof(char) * TCP_BUFFER_SIZE);
    while(1)
    {
        printf("Client:>");
        fgets(msg, TCP_BUFFER_SIZE, stdin);
        if(strcmp(msg, "quit\n") == 0)
        {
            ssize_t msglen = sendto(sockCli, msg, TCP_BUFFER_SIZE, 0, (struct sockaddr*)&addrSer, len);
            break;
        }
        ssize_t msglen = sendto(sockCli, msg, TCP_BUFFER_SIZE, 0, (struct sockaddr*)&addrSer, len);

        bzero(msg, TCP_BUFFER_SIZE);
        printf("Ser:>");
        recvfrom(sockCli, msg, TCP_BUFFER_SIZE, 0, (struct sockaddr*)&addrSer, &len);
        fputs(msg, stdout);
    }
    close(sockCli);
}

int main(int argc, char* argv[])
{
    if(argc <= 3)
    {
        printf("usage: %s -tcp/uudp  ip_address  portnumber\n", basename(argv[0]));
        return 1;
    }
    char* ip = argv[2];
    int port = atoi(argv[3]);
    int opt = 0;
    while((opt = getopt(argc, argv, "tu")) != -1)
    {
        switch(opt)
        {
            case 't':
                cli_tcp_communic(ip, port);
                break;
            case 'u':
                cli_udp_communic(ip, port);
                break;
            default:
                break;
        }
    }
    return 0;
}
