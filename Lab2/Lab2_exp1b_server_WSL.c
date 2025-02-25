// Lab2_exp1b_server_WSL.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // For close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>     // For sockaddr_in
#include <arpa/inet.h>      // For inet_ntoa()
#include <errno.h>          // For error handling

#define PORT 65431          // Port to listen on
#define BUFFER_SIZE 1024    // Buffer size for receiving messages

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address, client_address;
    socklen_t addr_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];
    int opt = 1;
    ssize_t bytes_read, bytes_sent;

    // 1. Create socket file descriptor (IPv4, TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set socket options (FIX: Remove SO_REUSEPORT)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Define the server address
    address.sin_family = AF_INET;          
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons(PORT);        

    // 4. Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 5. Listen for incoming connections
    if (listen(server_fd, 5) < 0) { 
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) { // Server runs indefinitely
        memset(&client_address, 0, sizeof(client_address)); // Reset client address structure

        // 6. Accept a new connection
        new_socket = accept(server_fd, (struct sockaddr *)&client_address, &addr_len);
        if (new_socket < 0) {
            perror("accept failed");
            continue;  // Don't exit, continue accepting new connections
        }

        printf("Connected to client: %s:%d\n",
               inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // 7. Communicate with the connected client
        while (1) {
            memset(buffer, 0, BUFFER_SIZE); // Clear the buffer

            // Receive data from the client
            bytes_read = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_read < 0) {
                perror("recv failed");
                break;
            } else if (bytes_read == 0) {
                printf("Client disconnected: %s:%d\n",
                       inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                break; // Client disconnected
            }

            buffer[bytes_read] = '\0'; // Null-terminate the received data
            printf("Received from client: %s\n", buffer);

            // Ensure safe concatenation to avoid buffer overflow
            size_t remaining_space = BUFFER_SIZE - bytes_read - 1;
            if (remaining_space > strlen(" Server reply")) {
                strcat(buffer, " Server reply");
            } else {
                fprintf(stderr, "Warning: Not enough space to append response\n");
            }

            // Send the modified message back to the client
            bytes_sent = send(new_socket, buffer, strlen(buffer), 0);
            if (bytes_sent < 0) {
                perror("send failed");
                break;
            }

            printf("Sent to client: %s\n", buffer);
        }

        // 8. Send one last message to the client before closing (Half-close demonstration)
        if (bytes_read == 0) { // Client closed connection on its side
            strcpy(buffer, "Server is sending a last message before closing from my side");
            bytes_sent = send(new_socket, buffer, strlen(buffer), 0);
            if (bytes_sent < 0) {
                perror("send failed");
                break;
            }            
            sleep(2); // Delay before closing
        }

        // Close the client socket
        close(new_socket);
    }

    // 9. Close the server socket (this part is never reached in an infinite loop)
    close(server_fd);
    return 0;
}