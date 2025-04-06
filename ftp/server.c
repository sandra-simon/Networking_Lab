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

void receiveFile(int newSock) {
    char buffer[BUFFER_SIZE];
    int bytesRead;
    FILE *fp = fopen("output.txt", "w");

    if (fp == NULL) {
        perror("File open error");
        return;
    }

    printf("Receiving file...\n");

    while ((bytesRead = recv(newSock, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), bytesRead, fp);
    }

    fclose(fp);

    printf("File received successfully.\n");

    // Now reverse the file contents
    FILE *rfp = fopen("output.txt", "r");
    fseek(rfp, 0, SEEK_END);
    long len = ftell(rfp);
    fseek(rfp, 0, SEEK_SET);

    char *content = malloc(len + 1);
    fread(content, 1, len, rfp);
    content[len] = '\0';

    fclose(rfp);

    // Reverse
    for (int i = 0, j = len - 1; i < j; i++, j--) {
        char temp = content[i];
        content[i] = content[j];
        content[j] = temp;
    }

    // Write reversed to file
    FILE *wfp = fopen("output.txt", "w");
    fwrite(content, 1, len, wfp);
    fclose(wfp);

    printf("Reversed File Contents:\n%s\n", content);

    free(content);
}

int main() {
    struct sockaddr_in servAddr;
    int serverSock, newSock;
    socklen_t addrSize;
    struct sockaddr_storage store;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(serverSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Binding failed");
        close(serverSock);
        exit(1);
    }

    if (listen(serverSock, 5) == 0) {
        printf("Server is listening...\n");
    } else {
        perror("Listen failed");
        close(serverSock);
        exit(1);
    }

    addrSize = sizeof(store);
    newSock = accept(serverSock, (struct sockaddr *)&store, &addrSize);
    if (newSock < 0) {
        perror("Accept failed");
        close(serverSock);
        exit(1);
    }

    printf("Client connected.\n");
    receiveFile(newSock);

    close(newSock);
    close(serverSock);
    return 0;
}
