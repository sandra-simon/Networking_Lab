// server.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>     // For socket(), bind(), accept(), recv()
#include <string.h>
#include <unistd.h>         // For close()
#include <netinet/in.h>     // For sockaddr_in
#include <arpa/inet.h>      // For inet_addr()

void receiveFile(int newSock) {
    char buffer[4098];
    int bytesRead = 0;
    int i, j;
    char temp;
    FILE *fp;

    fp = fopen("output.txt", "w");      // File to save reversed content
    if (fp == NULL) {
        printf("Error in Opening File\n");
        return;
    }

    // Receive data from client
    bytesRead = recv(newSock, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
        printf("Error receiving file or empty file.\n");
        fclose(fp);
        return;
    }

    buffer[bytesRead] = '\0';      // Null-terminate the received string

    // Reverse the string using two-pointer method
    i = 0;
    j = strlen(buffer) - 1;
    while (i < j) {
        temp = buffer[j];
        buffer[j] = buffer[i];
        buffer[i] = temp;
        i++;
        j--;
    }

    // Write reversed content to file
    fprintf(fp, "%s", buffer);
    printf("File Received and Reversed Successfully.\n");
    printf("Reversed Content: %s\n", buffer);
    fclose(fp);      // Close the output file
}

int main() {
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;
    int server, newSock;

    // Create a TCP socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Socket creation failed");
        return -1;
    }
    printf("Socket created.\n");

    // Server address setup
    servAddr.sin_family = AF_INET;               // IPv4
    servAddr.sin_port = htons(3621);             // Port number (3621)
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Bind to localhost
    memset(servAddr.sin_zero, 0, sizeof(servAddr.sin_zero)); // Padding

    // Bind the socket to the IP and port
    if (bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Binding failed");
        return -1;
    }
    printf("Binding done.\n");

    // Start listening for connections
    if (listen(server, 5) < 0) {
        perror("Listen failed");
        return -1;
    }
    printf("Server is listening...\n");

    addrSize = sizeof(store);

    // Accept a client connection
    newSock = accept(server, (struct sockaddr *)&store, &addrSize);
    if (newSock < 0) {
        perror("Accept failed");
        return -1;
    }
    printf("Client connected.\n");

    receiveFile(newSock);  // Handle receiving and reversing

    close(newSock);        // Close client socket
    close(server);         // Close server socket
    return 0;
}
