#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define MAX_PAYLOAD_SIZE 1000

// Structure to represent the packet format
struct Packet
{
    uint32_t sequence_number;       // Sequence number (4 bytes)
    uint8_t TTL;                    // Time-to-live (1 byte)
    uint16_t payload_length;        // Payload length (2 bytes)
    char payload[MAX_PAYLOAD_SIZE]; // Payload data (variable length)
};

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Usage: %s <ServerIP> <ServerPort> <P> <TTL> <output_file>\n", argv[0]);
        exit(1);
    }

    // Parse command-line arguments
    char *server_ip_str = argv[1];
    int server_port = atoi(argv[2]);
    int payload_size = atoi(argv[3]);
    int ttl = atoi(argv[4]);
    char *output_file = argv[5];

    // Validate inputs
    if (payload_size < 100 || payload_size > 1000 || ttl < 2 || ttl > 20 || ttl % 2 != 0)
    {
        printf("Invalid input parameters.\n");
        exit(1);
    }

    // Create socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // Server address
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    // Convert server IP address from presentation to network format
    struct in_addr server_addr;
    if (inet_pton(AF_INET, server_ip_str, &server_addr) <= 0)
    {
        perror("Invalid server IP address");
        exit(1);
    }

    server_address.sin_addr = server_addr;

    // Initialize cumulative RTT calculation variables
    struct timespec start_time, end_time;
    double cumulative_rtt = 0.0;

    // Open output file for saving Cumulative RTT values
    FILE *file = fopen(output_file, "a");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    // Loop to send packets
    //  num_packets = 50 here as in the question
    for (int i = 0; i < 50; i++)
    {
        // Create packet
        struct Packet packet;
        packet.sequence_number = i;                  // Sequence number
        packet.TTL = ttl;                            // TTL
        packet.payload_length = htons(payload_size); // Payload length
        // Fill payload with random data (for demonstration purposes)
        for (int j = 0; j < payload_size; j++)
        {
            packet.payload[j] = 'A' + (rand() % 26); // Random uppercase letter
        }

        // Record start time if it's the first packet
        if (i == 0)
        {
            // clock_gettime(CLOCK_MONOTONIC, &start_time);
            clock_t start_time = clock();
        }

        // Send packet to server
        sendto(client_socket, &packet, sizeof(packet), 0, (struct sockaddr *)&server_address, sizeof(server_address));

        // Receive packet back from server (not needed for this task)

        // Decrement TTL if not zero
        // the Client program decrements the TTL value and checks
        // if this new value is zero.
        if (packet.TTL > 0)
        {
            packet.TTL--;
        }

        // If TTL is zero, record end time and calculate cumulative RTT
        if (packet.TTL == 0)
        {
            // clock_gettime(CLOCK_MONOTONIC, &end_time);
            clock_t end_time = clock();
            
            // double rtt = (end_time.tv_sec - start_time.tv_sec) * 1000.0 + (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;
            double rtt = ((double)(end_time - start_time)/CLOCKS_PER_SEC) * 1000.00 ; 

            cumulative_rtt += rtt;
        }

        // Add delay between packets
        sleep(1); // Sleep for 1 seconds
    }

    // Save cumulative RTT to file
    fprintf(file, "%.2f\n", cumulative_rtt);

    // Close output file
    fclose(file);

    // Close socket
    close(client_socket);

    return 0;
}
