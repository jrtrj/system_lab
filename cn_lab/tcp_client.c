#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
int main()
{
    int sock;
    struct sockaddr_in server_address;
    socklen_t len;
    int i, f = 1, n, j;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    len = sizeof(server_address);
    connect(sock, (struct sockaddr*)&server_address, len);
    printf("Connection Request Sent\n");
    printf("ENTER THE Number\n");
    scanf("%d", &n);
    send(sock, &n, sizeof(n), 0);
    printf("Content Sent\n");
    recv(sock, &j, sizeof(j), 0);
    printf("%d\n", j);
    close(sock);
    return 0;
}
