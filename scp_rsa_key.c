#include <libssh/libssh.h>
#include <stdio.h>

int main() {
    ssh_session session;
    int verbosity = SSH_LOG_PROTOCOL;
    int port = 22;
    int rc;
    char *remote_path = "/path/to/remote/file";
    char *local_path = "/path/to/local/file";
    char *key_path = "/path/to/rsa/key";
    
    // Initialize SSH session
    session = ssh_new();
    if (session == NULL) {
        fprintf(stderr, "Error creating SSH session\n");
        return 1;
    }
    
    // Set SSH options
    ssh_options_set(session, SSH_OPTIONS_HOST, "remote-hostname");
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(session, SSH_OPTIONS_USER, "remote-username");
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(session, SSH_OPTIONS_IDENTITY, key_path);
    
    // Connect to remote host
    rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to remote host: %s\n", ssh_get_error(session));
        ssh_free(session);
        return 1;
    }
    
    // Authenticate with private key
    rc = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with private key: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }
    
    // Initialize SCP transfer
    ssh_scp scp = ssh_scp_new(session, SSH_SCP_WRITE, remote_path);
    if (scp == NULL) {
        fprintf(stderr, "Error initializing SCP transfer: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }
    
    // Start SCP transfer
    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error starting SCP transfer: %s\n", ssh_get_error(session));
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }
    
    // Open local file for reading
    FILE *local_file = fopen(local_path, "rb");
    if (local_file == NULL) {
        fprintf(stderr, "Error opening local file for reading\n");
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }
    
    // Transfer file contents
    char buffer[1024];
    size_t bytes;
    do {
        bytes = fread(buffer, 1, sizeof(buffer), local_file);
        if (ssh_scp_write(scp, buffer, bytes) != SSH_OK) {
            fprintf(stderr, "Error transferring file: %s\n", ssh_get_error(session));
            fclose(local_file);
            ssh_scp_free(scp);
            ssh_disconnect(session);
            ssh_free(session);
            return 1;
        }
    } while (bytes > 0);
    
    // Cleanup
    fclose(local_file);
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    ssh_disconnect(session);
    ssh_free(session);
    
    return 0;
}
