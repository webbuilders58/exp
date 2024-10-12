
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    // Chat loop
    while (1) {
        printf("Enter message: ");
        fgets(buffer, 1024, stdin);
        send(sock, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Closing connection\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, 1024);
        printf("Server: %s\n", buffer);
        
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Connection closed by server\n");
            break;
        }
    }

    close(sock);
    return 0;
}
