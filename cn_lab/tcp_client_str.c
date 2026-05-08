#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sock;
    struct sockaddr_in server_address;
    socklen_t len;
    char buffer[1024] = {0}; // Buffer to hold the string

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    len = sizeof(server_address);

    connect(sock, (struct sockaddr*)&server_address, len);
    printf("Connection Request Sent\n");

    printf("ENTER THE STRING:\n");
    // Read string from user
    fgets(buffer, sizeof(buffer), stdin);

    // Remove the newline character added by fgets
    buffer[strcspn(buffer, "\n")] = 0;

    // Send the string to the server
    send(sock, buffer, strlen(buffer), 0);
    printf("Content Sent\n");

    // Clear the buffer to receive the reversed string
    memset(buffer, 0, sizeof(buffer));

    // Receive the reversed string
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Reversed String from Server: %s\n", buffer);

    close(sock);
    return 0;
}
