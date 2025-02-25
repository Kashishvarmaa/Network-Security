#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 65431
#define BUFFER_SIZE 1024
#define MAX_THREADS 2  // Number of threads handling connections

typedef struct {
    int server_fd;
    struct sockaddr_in address;
} SocketInfo;

SocketInfo sock_info;
int message_counter = 1;  // Serial number for messages
pthread_mutex_t lock;

void *accept_new_connection(void *arg);

int main() {
    int opt = 1;
    pthread_t threads[MAX_THREADS];

    // 1. Create socket
    if ((sock_info.server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set socket options (FIX: Removed SO_REUSEPORT)
    if (setsockopt(sock_info.server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR failed");
        close(sock_info.server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Define server address
    sock_info.address.sin_family = AF_INET;
    sock_info.address.sin_addr.s_addr = INADDR_ANY;
    sock_info.address.sin_port = htons(PORT);

    // 4. Bind the socket
    if (bind(sock_info.server_fd, (struct sockaddr *)&sock_info.address, sizeof(sock_info.address)) < 0) {
        perror("Bind failed");
        close(sock_info.server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 5. Start listening before spawning threads
    if (listen(sock_info.server_fd, 5) < 0) {
        perror("Listen failed");
        close(sock_info.server_fd);
        exit(EXIT_FAILURE);
    }

    // 6. Initialize mutex
    pthread_mutex_init(&lock, NULL);

    // 7. Create threads for handling multiple connections
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, accept_new_connection, NULL);
    }

    // 8. Wait for threads to finish (this won't actually happen)
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 9. Cleanup
    pthread_mutex_destroy(&lock);
    close(sock_info.server_fd);
    return 0;
}

void *accept_new_connection(void *arg) {
    int new_socket;
    struct sockaddr_in client_address;
    socklen_t addr_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(&client_address, 0, sizeof(client_address)); // Reset client address structure

        // Accept a new connection
        addr_len = sizeof(client_address);
        new_socket = accept(sock_info.server_fd, (struct sockaddr *)&client_address, &addr_len);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;  // Don't exit, continue accepting new connections
        }

        printf("Connected to client: %s:%d\n",
               inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Handle communication
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);

            // Receive message from client
            ssize_t bytes_read = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_read <= 0) {
                printf("Client disconnected: %s:%d\n",
                       inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                break;
            }

            buffer[bytes_read] = '\0';
            printf("Received from client: %s\n", buffer);

            // Append serial number safely
            pthread_mutex_lock(&lock);
            snprintf(buffer, BUFFER_SIZE, "%d: Server Reply", message_counter++);
            pthread_mutex_unlock(&lock);

            // Send response
            ssize_t bytes_sent = send(new_socket, buffer, strlen(buffer), 0);
            if (bytes_sent < 0) {
                perror("Send failed");
                break;
            }

            printf("Sent to client: %s\n", buffer);
        }

        close(new_socket);
    }
    return NULL;
}