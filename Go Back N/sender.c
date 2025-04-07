#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TOTAL_PACKETS 10
#define TIMEOUT_SEC 3

int main() {
    int sockfd, base = 0, next_seq = 0, ack, window_size;
    struct sockaddr_in server_addr;
    fd_set readfds;
    struct timeval timeout;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6265);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server.\n");

    printf("Enter window size: ");
    scanf("%d", &window_size);


    while (base < TOTAL_PACKETS) {
        // Send all packets in the window
        for (int i = 0; i < window_size && (next_seq < TOTAL_PACKETS); i++) {
            printf("Sending packet: %d\n", next_seq);
            send(sockfd, &next_seq, sizeof(next_seq), 0);
            next_seq++;
        }

        // Wait for ACK with timeout
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;

        int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);

        if (activity > 0) {
            // Receive ACK
            recv(sockfd, &ack, sizeof(ack), 0);
            printf("Received ACK: %d\n", ack);
            base = ack;  // Slide the window
        } else {
            // Timeout occurred → Resend from base
            printf("Timeout occurred! Resending from packet %d\n", base);
            next_seq = base;
        }
    }

    close(sockfd);
    return 0;
}
