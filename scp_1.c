#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>

int main(int argc, char *argv[]) {
    ssh_session session;
    ssh_scp scp;
    int rc;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s username@remote_host:/remote/path /local/path\n", argv[0]);
        exit(1);
    }

    // Create SSH session
    session = ssh_new();
    if (session == NULL) {
        fprintf(stderr, "Error creating SSH session\n");
        exit(1);
    }

    // Set SSH options
    ssh_options_set(session, SSH_OPTIONS_HOST, argv[1]);
    ssh_options_set(session, SSH_OPTIONS_USER, strtok(argv[1], "@"));

    // Connect to remote host
    rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to remote host: %s\n", ssh_get_error(session));
        ssh_free(session);
        exit(1);
    }

    // Authenticate with private key
    rc = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with private key: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    // Create SCP transfer
    scp = ssh_scp_new(session, SSH_SCP_WRITE, argv[2]);
    if (scp == NULL) {
        fprintf(stderr, "Error creating SCP transfer: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    // Set SCP options
    ssh_scp_init(scp);
    rc = ssh_scp_push_file(scp, argv[3], ssh_filesize(argv[3]), 0644);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error transferring file: %s\n", ssh_get_error(session));
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    // Cleanup
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    ssh_disconnect(session);
    ssh_free(session);
    return 0;
}
