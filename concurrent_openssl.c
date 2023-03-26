#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void handle_client(SSL *ssl) {
    char buffer[1024];
    int n;
    
    // Authentication process
    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        return;
    }
    
    // Communication with client
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
    pid_t pid[5];
    int i;
    
    SSL_library_init();
    SSL_CTX *ctx = SSL_CTX_new(TLSv1_2_server_method());
    
    // Load certificate and private key
    SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM);
    SSL_CTX_check_private_key(ctx);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(server_socket, 5);
    
    for (i = 0; i < 5; i++) {
        address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &address_size);
        
        pid[i] = fork();
        if (pid[i] == 0) {
            // Child process code
            close(server_socket);
            SSL *ssl = SSL_new(ctx);
            SSL_set_fd(ssl, client_socket);
            handle_client(ssl);
        } else if (pid[i] > 0) {
            // Parent process code
            close(client_socket);
        } else {
            // Error handling code
            printf("Fork failed\n");
            exit(1);
        }
    }
    
    for (i = 0; i < 5; i++) {
        waitpid(pid[i], NULL, 0);
    }
    
    SSL_CTX_free(ctx);
    return 0;
}
In this example, we are using OpenSSL library to perform SSL/TLS based communication with clients. We are using fork to handle five concurrent client connections.

In the child process, we first perform the authentication process using SSL_accept() function. If the authentication fails, we print the error message and exit. Otherwise, we communicate with the client using SSL_read() and SSL_write() functions. After communication, we shutdown the SSL connection and exit.

In the parent process, we close the client socket and go back to listening for new connections.

Note that we are using SSL_CTX_new() function to create the SSL context, and loading the certificate and private key files using SSL_CTX_use_certificate_file() and SSL_CTX_use_PrivateKey_file() functions. We are also checking the validity of the private key using SSL_CTX_check_private_key() function.

After forking five child processes, we wait for them to exit using waitpid
