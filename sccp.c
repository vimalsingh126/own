#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>

#define BUFFER_SIZE 4096

int main() {
    ssh_session session;
    ssh_scp scp;
    char *local_file_path = "/path/to/local/file";
    char *remote_file_path = "/path/to/remote/file";
    char *remote_host = "remote_host";
    char *remote_user = "remote_user";
    char buffer[BUFFER_SIZE];
    int nbytes, rc;

    // Initialize the SSH session
    session = ssh_new();
    if (session == NULL) {
        fprintf(stderr, "Error: Failed to create SSH session.\n");
        exit(1);
    }

    // Set the SSH options
    ssh_options_set(session, SSH_OPTIONS_HOST, remote_host);
    ssh_options_set(session, SSH_OPTIONS_USER, remote_user);

    // Connect to the remote host
    rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error: Failed to connect to remote host: %s\n", ssh_get_error(session));
        exit(1);
    }

    // Authenticate with the remote host
    rc = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error: Failed to authenticate with remote host: %s\n", ssh_get_error(session));
        exit(1);
    }

    // Open an SCP session
    scp = ssh_scp_new(session, SSH_SCP_WRITE, remote_file_path);
    if (scp == NULL) {
        fprintf(stderr, "Error: Failed to create SCP session: %s\n", ssh_get_error(session));
        exit(1);
    }

    // Initiate the SCP session
    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error: Failed to initiate SCP session: %s\n", ssh_get_error(session));
        exit(1);
    }

    // Open the local file for reading
    FILE *local_file = fopen(local_file_path, "r");
    if (local_file == NULL) {
        fprintf(stderr, "Error: Failed to open local file for reading.\n");
        exit(1);
    }

    // Transfer the file contents to the remote host
    while ((nbytes = fread(buffer, 1, BUFFER_SIZE, local_file)) > 0) {
        rc = ssh_scp_write(scp, buffer, nbytes);
        if (rc != SSH_OK) {
            fprintf(stderr, "Error: Failed to transfer file to remote host: %s\n", ssh_get_error(session));
            exit(1);
        }
    }

    // Close the local file
    fclose(local_file);

    // Close the SCP session
    ssh_scp_close(scp);

    // Free the SCP session
    ssh_scp_free(scp);

    // Disconnect from the remote host
    ssh_disconnect(session);

    // Free the SSH session
    ssh_free(session);

    printf("File transfer complete.\n");

    return 0;
}
