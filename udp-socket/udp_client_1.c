#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>


// for this question we have to run it in 10.2.1.44 server port number 5555 ...

#define MAX_PAYLOAD_SIZE 1000

// Structure to represent the packet format

struct Packet
{
    uint32_t sequence_number; // Sequence number (4 bytes)
    uint8_t TTL; // Time-to-live (1 byte)
    uint16_t payload_length; // Payload length (2 bytes)
    char payload[MAX_PAYLOAD_SIZE]; // Payload data (variable length)
};

int main(int argc, char *argv[])

{

    if (argc != 6)

    {
        printf("Usage: <ServerIP> <ServerPort> <P> <TTL> <NumPackets>\n");
        exit(1);
    }

    // Parse command-line arguments

    char *server_ip_str = argv[1];
    int server_port = atoi(argv[2]);
    int payload_size = atoi(argv[3]);
    int ttl = atoi(argv[4]);
    int num_packets = atoi(argv[5]);

    // Validate inputs

    if (payload_size < 100 || payload_size > 1000 || ttl < 2 || ttl > 20 || ttl % 2 != 0 || num_packets < 1 || num_packets > 50)

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
    // Loop to send packets
    struct timeval start_time, end_time;

    double total_rtt = 0.0; // Total round trip time

    for (int i = 0; i < num_packets; ++i)

    {

        // Create packet
        struct Packet packet;
        packet.sequence_number = i; // Sequence number
        packet.TTL = ttl; // TTL
        packet.payload_length = htons(payload_size); // Payload length
        // Fill payload with random data (for demonstration purposes)

        for (int j = 0; j < payload_size; ++j)

        {

            packet.payload[j] = 'A' + (rand() % 26); // Random uppercase letter
        }

        // Print payload before sending

        printf("Sending payload: %s\n\n", packet.payload);

        // Record start time

        gettimeofday(&start_time, NULL);

        // Send packet to server

        sendto(client_socket, &packet, sizeof(packet), 0, (struct sockaddr *)&server_address, sizeof(server_address));

        // Receive packet back from server

        recvfrom(client_socket, &packet, sizeof(packet), 0, NULL, NULL);

        // Print payload before sending

        printf("Received payload: %s\n\n", packet.payload);

        // Record end time

        gettimeofday(&end_time, NULL);

        // Calculate RTT for this packet

        double rtt = (end_time.tv_sec - start_time.tv_sec) * 1000.0 + (end_time.tv_usec - start_time.tv_usec) / 1000.0;

        total_rtt += rtt;

        printf("RTT for packet %d: %.2f ms\n\n", i, rtt);

        sleep(1); // Add delay between packets (for demonstration purposes)
    }

    // Calculate and print average RTT across all packets

    printf("Average RTT: %.2f ms\n\n", total_rtt / num_packets);

    close(client_socket);

    return 0;
}



// Record start time
    clock_t start_time = clock();

    // Record end time
    clock_t end_time = clock();

    // Calculate RTT for this packet
    double rtt = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;