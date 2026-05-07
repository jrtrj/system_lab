#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main()
{
int sock;
int num, result;
struct sockaddr_in server_address;
socklen_t len;
sock = socket(AF_INET, SOCK_DGRAM, 0);
if(sock < 0)
{
perror("Socket error");
exit(EXIT_FAILURE);
}
server_address.sin_family = AF_INET;
server_address.sin_port = htons(8080);
server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
len = sizeof(server_address);
printf("Enter number: ");
scanf("%d", &num);
sendto(sock, &num, sizeof(num), 0, (struct sockaddr*)&server_address, len);
recvfrom(sock, &result, sizeof(result), 0, (struct sockaddr*)&server_address, &len);
printf("Factorial: %d\n", result);
close(sock);
return 0;
}
