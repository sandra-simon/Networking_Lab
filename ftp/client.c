#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3621
#define BUFFER_SIZE 4096

void sendFile(int clientSock) {
    char buffer[BUFFER_SIZE], filename[128];
    int bytesRead;
    FILE *fp;

    printf("Enter the filename: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File open error");
        return;
    }

    printf("Sending file...\n");

    while ((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
        if (send(clientSock, buffer, bytesRead, 0) < 0) {
            perror("Send error");
            break;
        }
    }

    fclose(fp);
    printf("File sent successfully.\n");
}

int main() {
    int clientSock;
    struct sockaddr_in servAddr;

    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Connection failed");
        close(clientSock);
        exit(1);
    }

    printf("Connected to server.\n");

    sendFile(clientSock);

    close(clientSock);
    return 0;
}
