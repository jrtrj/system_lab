#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_sock, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t len;
    char buffer[1024] = {0}; // Buffer to hold the string
    int i, string_len;
    char temp;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(server_sock, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_sock, 5);
    printf("Listening...\n");

    len = sizeof(client_address);
    client_socket = accept(server_sock, (struct sockaddr*)&client_address, &len);
    printf("Connection Established\n");

    // Clear buffer and receive string from client
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("String received: %s\n", buffer);

    // Reverse the string
    string_len = strlen(buffer);
    for(i = 0; i < string_len / 2; i++)
    {
        temp = buffer[i];
        buffer[i] = buffer[string_len - 1 - i];
        buffer[string_len - 1 - i] = temp;
    }

    // Send the reversed string back to the client
    send(client_socket, buffer, strlen(buffer), 0);
    printf("Reversed Content Sent\n");

    close(client_socket);
    close(server_sock);
    return 0;
}
