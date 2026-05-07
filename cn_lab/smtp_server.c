#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];
    char mailData[BUFFER_SIZE];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket failed");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(serverSocket, 5);
    printf("SMTP Server listening on port %d...\n", PORT);

    addr_size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);

    send(clientSocket, "220 Service Ready\n", 19, 0);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(clientSocket, buffer, BUFFER_SIZE, 0);

        printf("Client: %s", buffer);

        if (strncmp(buffer, "HELO", 4) == 0) {
            send(clientSocket, "250 Hello\n", 10, 0);
        }
        else if (strncmp(buffer, "MAIL FROM", 9) == 0) {
            send(clientSocket, "250 OK\n", 7, 0);
        }
        else if (strncmp(buffer, "RCPT TO", 7) == 0) {
            send(clientSocket, "250 OK\n", 7, 0);
        }
        else if (strncmp(buffer, "DATA", 4) == 0) {
            send(clientSocket, "354 Start mail input; end with .\n", 33, 0);

            memset(mailData, 0, BUFFER_SIZE);
            recv(clientSocket, mailData, BUFFER_SIZE, 0);

            printf("Mail Content:\n%s\n", mailData);

            send(clientSocket, "250 Message received\n", 21, 0);
        }
        else if (strncmp(buffer, "QUIT", 4) == 0) {
            send(clientSocket, "221 Service closing\n", 20, 0);
            break;
        }
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
