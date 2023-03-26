#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 3

void handle_client(int client_socket) {
    char buffer[1024];
    int n;
    
    while ((n = read(client_socket, buffer, 1024)) > 0) {
        write(client_socket, buffer, n);
    }
    
    close(client_socket);
    exit(0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    pid_t pid[MAX_CLIENTS];
    socklen_t address_size;
    int i = 0;
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(server_socket, 5);
    
    while (i < MAX_CLIENTS) {
        address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &address_size);
        
        pid[i] = fork();
        if (pid[i] == 0) {
            // Child process code
            close(server_socket);
            handle_client(client_socket);
        } else if (pid[i] > 0) {
            // Parent process code
            close(client_socket);
            i++;
        } else {
            // Error handling code
            printf("Fork failed\n");
            exit(1);
        }
    }
    
    for (i = 0; i
