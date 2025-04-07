#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define TOTAL_PACKETS 10

int main() {
    int server_fd, client_fd, packet;
    int received[TOTAL_PACKETS] = {0};
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    srand(time(0)); // For random drop

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6265);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 1);
    printf("Receiver is listening...\n");

    addr_size = sizeof(server_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&server_addr, &addr_size);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Sender connected.\n");

    int total_received = 0;

    while (total_received < TOTAL_PACKETS) {
        if (recv(client_fd, &packet, sizeof(packet), 0) > 0) {
            if ((rand() % 10) < 2) {  // Simulate 20% packet drop
                printf("Simulated drop for packet %d\n", packet);
                continue;
            }

            if (!received[packet]) {
                received[packet] = 1;
                total_received++;
                printf("Received new packet: %d\n", packet);
            } else {
                printf("Duplicate packet received: %d\n", packet);
            }

            // Send individual ACK
            send(client_fd, &packet, sizeof(packet), 0);
            printf("Sent ACK: %d\n", packet);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
