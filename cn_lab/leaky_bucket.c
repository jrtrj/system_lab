#include <stdio.h>

int main() {
    int bucket_size, output_rate, n;
    int i, packet_size;
    int bucket_content = 0;

    printf("Enter bucket size: ");
    scanf("%d", &bucket_size);

    printf("Enter output rate: ");
    scanf("%d", &output_rate);

    printf("Enter number of packets: ");
    scanf("%d", &n);

    for(i = 1; i <= n; i++) {
        printf("\nEnter size of packet %d: ", i);
        scanf("%d", &packet_size);

        if(packet_size + bucket_content <= bucket_size) {
            bucket_content += packet_size;
            printf("Packet accepted. Bucket content = %d\n", bucket_content);
        } else {
            printf("Packet dropped (Overflow)\n");
        }

        if(bucket_content >= output_rate)
            bucket_content -= output_rate;
        else
            bucket_content = 0;

        printf("After leaking, bucket content = %d\n", bucket_content);
    }
    return 0;
}
