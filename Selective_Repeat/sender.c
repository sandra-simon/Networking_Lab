#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define TOTAL_PACKETS 10
#define TIMEOUT_SEC 3

int main() {
    int sockfd, window_size;
    struct sockaddr_in server_addr;
    int ack[TOTAL_PACKETS] = {0}; // Track ACKs for each packet
    int timers[TOTAL_PACKETS] = {0}; // Track if a timer is active for a packet
    int base = 0;

    printf("Enter window size: ");
    scanf("%d", &window_size);

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

    fd_set readfds;
    struct timeval timeout;

    int time_sent[TOTAL_PACKETS] = {0};
    int seq = 0;

    while (base < TOTAL_PACKETS) {
        // Send packets in the window
        for (int i = 0; i < window_size && (base + i) < TOTAL_PACKETS; i++) {
            int pkt = base + i;
            if (!ack[pkt] && !timers[pkt]) {
                printf("Sending packet: %d\n", pkt);
                send(sockfd, &pkt, sizeof(pkt), 0);
                timers[pkt] = 1;
                time_sent[pkt] = time(NULL);
            }
        }

        // Wait for ACKs
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        if (activity > 0) {
            int received_ack;
            recv(sockfd, &received_ack, sizeof(received_ack), 0);
            printf("Received ACK: %d\n", received_ack);
            ack[received_ack] = 1;
            timers[received_ack] = 0;

            // Slide window base
            while (ack[base] && base < TOTAL_PACKETS)
                base++;
        }

        // Resend timed out packets
        for (int i = base; i < base + window_size && i < TOTAL_PACKETS; i++) {
            if (timers[i] && (time(NULL) - time_sent[i] >= TIMEOUT_SEC)) {
                printf("Timeout for packet %d, resending...\n", i);
                send(sockfd, &i, sizeof(i), 0);
                time_sent[i] = time(NULL);
            }
        }
    }

    close(sockfd);
    return 0;
}
