#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>

int main() {
    ssh_session session;
    int port = 22;

    char* user = "username"; //replace with your username
    char* password = "password"; //replace with your password
    char* host = "192.168.1.10"; //replace with the IP address of the remote host
    char* local_path = "/home/user/localfile.txt"; //replace with the path of the local file to be transferred
    char* remote_path = "/home/user/remotefile.txt"; //replace with the path of the remote file to be created

    ssh_options_set(session, SSH_OPTIONS_HOST, host);
    ssh_options_set(session, SSH_OPTIONS_USER, user);
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);

    session = ssh_new();
    if (session == NULL) {
        fprintf(stderr, "Error creating SSH session\n");
        exit(1);
    }

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to SSH server: %s\n", ssh_get_error(session));
        ssh_free(session);
        exit(1);
    }

    rc = ssh_userauth_password(session, NULL, password);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    ssh_scp scp = ssh_scp_new(session, SSH_SCP_WRITE, remote_path);
    if (scp == NULL) {
        fprintf(stderr, "Error creating SCP session: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error initializing SCP session: %s\n", ssh_get_error(session));
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    FILE *local_file = fopen(local_path, "rb");
    if (local_file == NULL) {
        fprintf(stderr, "Error opening local file: %s\n", local_path);
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    struct stat fileinfo;
    stat(local_path, &fileinfo);
    rc = ssh_scp_push_file(scp, remote_path, fileinfo.st_size, S_IRUSR | S_IWUSR);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error pushing file to remote host: %s\n", ssh_get_error(session));
        fclose(local_file);
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    char buffer[8192];
    size_t nbytes;
    while ((nbytes = fread(buffer, 1, sizeof(buffer), local_file)) > 0) {
        if (ssh_scp_write(scp, buffer, nbytes) != SSH_OK) {
            fprintf(stderr, "Error writing file to remote host: %s\n", ssh_get_error(session

                                                                                     
                                                                                     
                                                                                    
/*
#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>
#include <sys/stat.h>

int scp_to_remote(ssh_session session, const char *local_path, const char *remote_path) {
    ssh_scp scp = ssh_scp_new(session, SSH_SCP_WRITE, remote_path);
    if (scp == NULL) {
        fprintf(stderr, "Error: Failed to create SCP session: %s\n", ssh_get_error(session));
        return SSH_ERROR;
    }
    
    if (ssh_scp_init(scp) != SSH_OK) {
        fprintf(stderr, "Error: Failed to initialize SCP session: %s\n", ssh_get_error(session));
        ssh_scp_free(scp);
        return SSH_ERROR;
    }

    struct stat file_stat;
    if (stat(local_path, &file_stat) == -1) {
        fprintf(stderr, "Error: Failed to get file information: %s\n", strerror(errno));
        ssh_scp_free(scp);
        return SSH_ERROR;
    }
    ssh_scp_push_file(scp, local_path, file_stat.st_size, S_IRUSR | S_IWUSR);
    ssh_scp_close(scp);
    ssh_scp_free(scp);

    return SSH_OK;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <username> <password> <local_file_path> <remote_file_path>\n", argv[0]);
        return 1;
    }

    ssh_session session = ssh_new();
    if (session == NULL) {
        fprintf(stderr, "Error: Failed to create SSH session\n");
        return 1;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, "remote_host");
    ssh_options_set(session, SSH_OPTIONS_USER, argv[1]);

    if (ssh_connect(session) != SSH_OK) {
        fprintf(stderr, "Error: Failed to connect to SSH server: %s\n", ssh_get_error(session));
        ssh_free(session);
        return 1;
    }

    if (ssh_userauth_password(session, NULL, argv[2]) != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error: Authentication failed: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    int ret = scp_to_remote(session, argv[3], argv[4]);

    ssh_disconnect(session);
    ssh_free(session);

    return ret;
}
*/
