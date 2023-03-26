#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAX_CLIENTS 100

void handle_client(SSL *ssl) {
    char buffer[1024];
    int bytes_read;

    while (1) {
        bytes_read = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            // Connection closed or error occurred
            break;
        }
        SSL_write(ssl, buffer, bytes_read);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    exit(0);
}

int main(int argc, char *argv[]) {
    int server_fd, client_fd, pid;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    SSL_CTX *ctx;
    SSL *ssl;
    int client_sockets[MAX_CLIENTS];
    int num_clients = 0;

    // Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLSv1_2_server_method());
    if (ctx == NULL) {
        fprintf(stderr, "Error creating SSL context\n");
        exit(1);
    }

    // Load server certificate and private key
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        fprintf(stderr, "Error loading server certificate file\n");
        exit(1);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        fprintf(stderr, "Error loading server private key file\n");
        exit(1);
    }

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr, "Error creating server socket: %s\n", strerror(errno));
        exit(1);
    }

    // Bind server socket to address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Error binding server socket: %s\n", strerror(errno));
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        fprintf(stderr, "Error listening for incoming connections: %s\n", strerror(errno));
        close(server_fd);
        exit(1);
    }

    // Handle incoming connections
    while (1) {
        // Accept incoming connection
        client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            fprintf(stderr, "Error accepting incoming connection: %s\n", strerror(errno));
            continue;
        }

        // Fork child process to handle client connection
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Error forking child process: %s\n", strerror(errno));
            close(client_fd);
            continue;
        }
        if (pid == 0) {
            // Child process
            close(server_fd);

            // Initialize SSL connection
            ssl = SSL_new(ctx);
            SSL_set_fd

              
              
              #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAX_CLIENTS 100

void handle_client(SSL *ssl) {
    char buffer[1024];
    int bytes_read;

    while (1) {
        bytes_read = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            // Connection closed or error occurred
            break;
        }
        SSL_write(ssl, buffer, bytes_read);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    exit(0);
}

int main(int argc, char *argv[]) {
    int server_fd, client_fd, pid;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    SSL_CTX *ctx;
    SSL *ssl;
    int client_sockets[MAX_CLIENTS];
    int num_clients = 0;

    // Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLSv1_2_server_method());
    if (ctx == NULL) {
        fprintf(stderr, "Error creating SSL context\n");
        exit(1);
    }

    // Load server certificate and private key
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        fprintf(stderr, "Error loading server certificate file\n");
        exit(1);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        fprintf(stderr, "Error loading server private key file\n");
        exit(1);
    }

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr, "Error creating server socket: %s\n", strerror(errno));
        exit(1);
    }

    // Bind server socket to address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Error binding server socket: %s\n", strerror(errno));
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        fprintf(stderr, "Error listening for incoming connections: %s\n", strerror(errno));
        close(server_fd);
        exit(1);
    }

    // Handle incoming connections
    while (1) {
        // Accept incoming connection
        client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            fprintf(stderr, "Error accepting incoming connection: %s\n", strerror(errno));
            continue;
        }

        // Fork child process to handle client connection
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Error forking child process: %s\n", strerror(errno));
            close(client_fd);
            continue;
        }
        if (pid == 0) {
            // Child process
            close(server_fd);

            // Initialize SSL connection
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl,
