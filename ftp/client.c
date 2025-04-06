// client.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>     // For socket(), connect(), send()
#include <string.h>
#include <unistd.h>         // For close()
#include <netinet/in.h>     // For sockaddr_in
#include <arpa/inet.h>      // For inet_addr()

void sendFile(int client) {
    char buffer[4098];             // Buffer to hold file content chunks
    char filename[128];            // To store filename input from user
    int bytesRead = 0;
    FILE *fp;

    printf("Enter the Filename: ");
    scanf("%s", filename);         // Get filename from user

    fp = fopen(filename, "r");     // Open file in read mode
    if (fp == NULL) {
        printf("Error in opening file.\n");
        return;
    }

    // Read the file and send to server in chunks
    while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer) - 1, fp)) > 0) {
        buffer[bytesRead] = '\0';                  // Null terminate the buffer
        send(client, buffer, strlen(buffer), 0);   // Send the data over socket
    }

    printf("File '%s' sent successfully.\n", filename);
    fclose(fp);        // Close file after sending
}

int main() {
    int client;
    struct sockaddr_in servAddr;
    socklen_t addrSize;

    // Create a TCP socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        perror("Socket creation failed");
        return -1;
    }
    printf("Socket created.\n");

    // Server address setup
    servAddr.sin_family = AF_INET;                 // IPv4
    servAddr.sin_port = htons(3621);               // Port number (3621)
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost
    memset(servAddr.sin_zero, 0, sizeof(servAddr.sin_zero));  // Zero padding
    addrSize = sizeof(servAddr);

    // Connect to the server
    if (connect(client, (struct sockaddr *)&servAddr, addrSize) < 0) {
        perror("Connection failed");
        return -1;
    }
    printf("Connected to server.\n");

    sendFile(client);      // Send file content to server
    close(client);         // Close the socket
    return 0;
}
