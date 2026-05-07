#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main()
{
    int sock;
    int num, fact, i;
    struct sockaddr_in server_address, client_address;
    socklen_t len;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
    perror("Socket error");
    exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if(bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
    perror("Bind failed");
    exit(EXIT_FAILURE);
    }
    len = sizeof(client_address);
    recvfrom(sock, &num, sizeof(num), 0, (struct sockaddr*)&client_address, &len);
    printf("Received: %d\n", num);
    fact = 1;
    for(i = 1; i <= num; i++)
    {
    fact *= i;
    }
    sendto(sock, &fact, sizeof(fact), 0, (struct sockaddr*)&client_address, len);
    printf("Factorial sent\n");
    close(sock);
    return 0;
}
