// Lab 3: Exp2 Asymmetric Key RSA
// Before running the below command make sure the open SSL lib is installed on WSL
// sudo apt-get update
// sudo apt-get install libssl-dev
// Before running this program, generate both private.pem and public.pem on WSL side
// Use the below command for private.pem
// openssl genpkey -algorithm RSA -out private.pem -pkeyopt rsa_keygen_bits:2048
// Use the below command for public.pem
// openssl rsa -pubout -in private.pem -out public.pem
// Move the public.pem file from WSL to Windows directory where the client Python is running from
// Then, give the below command on WSL to build the executable
// gcc Lab3_Exp2_Asym_Server_WSL.c -lssl -lcrypto -o asym_server -Wno-deprecated-declarations
// Now run it using: ./asym_server first before running the client program on Windows

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#define PORT 65432
#define BUFFER_SIZE 512

// Load RSA private key from file
RSA* load_private_key(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) { perror("fopen private.pem"); exit(EXIT_FAILURE); }
    RSA* rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!rsa) { ERR_print_errors_fp(stderr); exit(EXIT_FAILURE); }
    return rsa;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addr_len = sizeof(address);
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read;
    
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    
    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(EXIT_FAILURE); }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind"); exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0) { perror("listen"); exit(EXIT_FAILURE); }
    printf("Asymmetric server listening on port %d...\n", PORT);
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addr_len);
    if (new_socket < 0) { perror("accept"); exit(EXIT_FAILURE); }
    
    bytes_read = read(new_socket, buffer, BUFFER_SIZE);
    
    RSA* rsa = load_private_key("private.pem");
    unsigned char decrypted[BUFFER_SIZE];
    int decrypted_len = RSA_private_decrypt(bytes_read, buffer, decrypted, rsa, RSA_PKCS1_PADDING);
    if (decrypted_len == -1) { ERR_print_errors_fp(stderr); exit(EXIT_FAILURE); }
    decrypted[decrypted_len] = '\0';
    printf("Decrypted message from client: %s\n", decrypted);
    
    // Prepare response message and encrypt with the public key.
    // For simplicity, assume the client's public key is the same as the server's public key.
    FILE *fp = fopen("public.pem", "r");
    if (!fp) { perror("fopen public.pem"); exit(EXIT_FAILURE); }
    RSA* rsa_pub = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    fclose(fp);
    char *response = "Hello from asymmetric server";
    unsigned char encrypted[BUFFER_SIZE];
    int encrypted_len = RSA_public_encrypt(strlen(response), (unsigned char*)response, encrypted, rsa_pub, RSA_PKCS1_PADDING);
    if (encrypted_len == -1) { ERR_print_errors_fp(stderr); exit(EXIT_FAILURE); }
    
    write(new_socket, encrypted, encrypted_len);
    
    RSA_free(rsa);
    RSA_free(rsa_pub);
    close(new_socket);
    close(server_fd);
    EVP_cleanup();
    ERR_free_strings();
    return 0;
}
