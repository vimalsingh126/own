#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define CERT_FILE "server.crt"
#define KEY_FILE "server.key"

void handle_client(SSL *ssl) {
    char buffer[1024];
    int n;

    // Authenticate client
    SSL_CTX_set_verify(SSL_get_SSL_CTX(ssl), SSL_VERIFY_PEER, NULL);
    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        exit(1);
    }

    // Read and write data to client
    while ((n = SSL_read(ssl, buffer, 1024)) > 0) {
        SSL_write(ssl, buffer, n);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    exit(0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t address_size;
    pid_t pid;
    SSL_CTX *ctx;
    SSL *ssl;

    // Initialize OpenSSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_server_method());

    // Load server certificate and private key
    SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM);
    if (!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr, "Private key does not match the public certificate\n");
        exit(1);
    }

    // Create server socket and bind to address
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    while (1) {
        // Accept incoming client connection
        address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &address_size);

        // Fork child process to handle client connection
        pid = fork();
        if (pid == 0) {
            // Child process code
            close(server_socket);

            // Create SSL connection
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, client_socket);

            handle_client(ssl);
        } else if (pid > 0) {
            // Parent process code
            close(client_socket);
        } else {
            // Error handling code
            printf("Fork failed\n");
            exit(1);
        }
    }

    // Clean up OpenSSL
    SSL_CTX_free(ctx);
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
In this example, we first initialize OpenSSL and load the server's certificate and private key. We then create a server socket and listen for incoming client connections. When a client connects, we fork a new process and create an SSL connection using the client socket. We then authenticate the client by setting the SSL_VERIFY_PEER flag and calling SSL_accept(). If authentication fails, we close the SSL connection and exit the child process. If authentication succeeds, we handle the client's request by reading and writing data using SSL_read() and SSL_write(). Once the client
