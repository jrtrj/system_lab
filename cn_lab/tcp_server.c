#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
int main()
{
int server_sock, client_socket;
struct sockaddr_in server_address, client_address;
socklen_t len;
int i, f = 1, n, j;
server_sock = socket(AF_INET, SOCK_STREAM, 0);
server_address.sin_family = AF_INET;
server_address.sin_port = htons(8080);
server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
bind(server_sock, (struct sockaddr*)&server_address, sizeof(server_address));
listen(server_sock, 5);
printf("Listening\n");
len = sizeof(client_address);
client_socket = accept(server_sock, (struct sockaddr*)&client_address, &len);
printf("Connection Established\n");
recv(client_socket, &n, sizeof(n), 0);
for(i = 1; i <= n; i++)
{
f = f * i;
}
send(client_socket, &f, sizeof(f), 0);
printf("Content Sent\n");
close(client_socket);
close(server_sock);
return 0;
}
