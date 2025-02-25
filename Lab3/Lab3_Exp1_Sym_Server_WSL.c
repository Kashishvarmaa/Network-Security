// Lab 3: Exp1 Server Symmetric Key AES
// Before running the below command make sure the OpenSSL lib is installed on WSL
// sudo apt-get update
// sudo apt-get install libssl-dev
// On WSL give the below command to compile and build the executable
// gcc Lab3_Exp1_Sym_Server_WSL.c -lssl -lcrypto -o sym_server 
// Now run it using: ./sym_server first before running the client program on Windows

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define PORT 65431
#define BUFFER_SIZE 1024

// For AES-128, key and IV are 16 bytes.
unsigned char key[16] = "0123456789abcdef";
unsigned char iv[16]  = "abcdef9876543210";

// Decrypt ciphertext using AES-128-CBC
int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) { ERR_print_errors_fp(stderr); exit(1); }
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        ERR_print_errors_fp(stderr); exit(1);
    }
    int len, plaintext_len;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
        ERR_print_errors_fp(stderr); exit(1);
    }
    plaintext_len = len;
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        ERR_print_errors_fp(stderr); exit(1);
    }
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

// Encrypt plaintext using AES-128-CBC
int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) { ERR_print_errors_fp(stderr); exit(1); }
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        ERR_print_errors_fp(stderr); exit(1);
    }
    int len, ciphertext_len;
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
        ERR_print_errors_fp(stderr); exit(1);
    }
    ciphertext_len = len;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        ERR_print_errors_fp(stderr); exit(1);
    }
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    unsigned char buffer[BUFFER_SIZE];
    unsigned char decrypted[BUFFER_SIZE];
    unsigned char encrypted[BUFFER_SIZE];

    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed"); exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed"); exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0) {
        perror("listen failed"); exit(EXIT_FAILURE);
    }
    printf("Symmetric server listening on port %d...\n", PORT);
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);
    if (new_socket < 0) { perror("accept failed"); exit(EXIT_FAILURE); }
    
    // Read encrypted message from client
    int bytes_read = read(new_socket, buffer, BUFFER_SIZE);
    int decrypted_len = aes_decrypt(buffer, bytes_read, decrypted);
    decrypted[decrypted_len] = '\0';
    printf("Decrypted message from client: %s\n", decrypted);
    
    // Prepare a response
    char *response = "Hello from symmetric server";
    int resp_len = aes_encrypt((unsigned char*)response, strlen(response), encrypted);
    write(new_socket, encrypted, resp_len);
    
    close(new_socket);
    close(server_fd);
    EVP_cleanup();
    ERR_free_strings();
    return 0;
}