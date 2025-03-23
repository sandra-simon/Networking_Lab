#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#define BUCKET_SIZE 10
//#define OUTPUT_RATE 4

int BUCKET_SIZE=0;
int OUTPUT_RATE=0;
void leaky_bucket(int packets[], int n) {
    int bucket = 0;

    printf("\nTime Step | Incoming | Bucket Size | Sent | Remaining\n");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-9d | %-8d | ", i + 1, packets[i]);

        if (packets[i] + bucket > BUCKET_SIZE) {
            printf("Overflow!   | ");
            bucket = BUCKET_SIZE;
        } else {
            bucket += packets[i];
            printf("%-11d | ", bucket);
        }
        int sent = (bucket < OUTPUT_RATE) ? bucket : OUTPUT_RATE;
        bucket -= sent;
        printf("%-4d | %-8d\n", sent, bucket);
        sleep(1);
    }
    while (bucket > 0) {
        int sent = (bucket < OUTPUT_RATE) ? bucket : OUTPUT_RATE;
        bucket -= sent;
        printf("%-9s | %-8s | %-11d | %-4d | %-8d\n", "Drain", "--", bucket + sent, sent, bucket);
        sleep(1);
    }
}


int main() {
    int n;
    printf("Enter bucket size : ");
    scanf("%d", &BUCKET_SIZE);
    printf("Enter output rate : ");
    scanf("%d", &OUTPUT_RATE);
    printf("Enter the number of time steps: ");
    scanf("%d", &n);
    int packets[n];
    printf("Enter the incoming packets at each time step:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &packets[i]);
    }
    leaky_bucket(packets, n);
    return 0;
}
