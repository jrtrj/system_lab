#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server: %s", buffer);

    send(sock, "HELO localhost\n", 15, 0);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server: %s", buffer);

    send(sock, "MAIL FROM: sender@test.com\n", 28, 0);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server: %s", buffer);

    send(sock, "RCPT TO: receiver@test.com\n", 28, 0);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server: %s", buffer);

    send(sock, "DATA\n", 5, 0);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server: %s", buffer);

    send(sock, "Hello, this is a test mail.\n.\n", 31, 0);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server: %s", buffer);

    send(sock, "QUIT\n", 5, 0);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server: %s", buffer);

    close(sock);
    return 0;
}
