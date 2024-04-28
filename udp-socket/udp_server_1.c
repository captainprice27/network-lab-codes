#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

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

    if (argc != 2) // command line inputs

    {

        printf("Usage: %s <ServerPort>\n", argv[0]);

        exit(1);
    }

    // server port   numebr conversion

    int server_port = atoi(argv[1]);

    // Create socket

    int server_socket = socket(AF_INET, SOCK_DGRAM, 0); // DGRAM for udp

    if (server_socket < 0)

    {

        perror("Socket creation failed");

        exit(1);
    }

    // error checking comments

    // printf("ok till now 1 \n");

    // Server address

    struct sockaddr_in server_address, client_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;

    server_address.sin_port = htons(server_port);

    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Server is listening on port %d\n", server_port);

    // Bind socket to address

    int bind_val = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)

    if (bind_val < 0)

    {

        perror("Binding failed");

        exit(1);
    }

    // error checking comments

    // printf("ok till now 2");

    // Loop to receive and forward packets

    struct Packet packet;

    socklen_t client_address_len = sizeof(client_address);

    while (1)

    {

        // Receive packet from client

        ssize_t bytes_received = recvfrom(server_socket, &packet, sizeof(packet), 0, (struct sockaddr *)&client_address, &client_address_len);

        if (bytes_received < 0)

        {

            perror("Error receiving packet");

            continue; // Continue to next iteration of the loop
        }

        // Check payload length

        uint16_t payload_length = ntohs(packet.payload_length);

        if (payload_length > MAX_PAYLOAD_SIZE)

        {

            printf("MALFORMED PACKET: Payload length too large\n");

            continue; // Continue to next iteration of the loop
        }

        // Decrement TTL

        if (packet.TTL > 0)

        {

            packet.TTL--;
        }

        // Forward packet back to client

        sendto(server_socket, &packet, bytes_received, 0, (struct sockaddr *)&client_address, sizeof(client_address));
    }

    close(server_socket);

    return 0;
}
