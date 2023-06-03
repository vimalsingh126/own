#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>

int main() {
    ssh_session session = ssh_new();
    ssh_options_set(session, SSH_OPTIONS_HOST, "hostname");
    ssh_options_set(session, SSH_OPTIONS_USER, "username");

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to the SSH server: %s\n", ssh_get_error(session));
        ssh_free(session);
        return 1;
    }

    rc = ssh_userauth_password(session, NULL, "password");
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with the SSH server: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    ssh_scp scp = ssh_scp_new(session, SSH_SCP_WRITE, "/path/to/destination");
    if (scp == NULL) {
        fprintf(stderr, "Error creating SCP session: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error initializing SCP transfer: %s\n", ssh_get_error(session));
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // Add files or directories to the SCP transfer
    rc = ssh_scp_push_file(scp, "/path/to/source/file", file_size, S_IRUSR | S_IWUSR);
    // Add more files or directories as needed

    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE) {
        fprintf(stderr, "Error starting SCP transfer: %s\n", ssh_get_error(session));
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    while (ssh_scp_pull_request(scp) == SSH_SCP_REQUEST_NEWFILE) {
        char buffer[8192];
        int bytesRead;

        ssh_scp_accept_request(scp);

        while ((bytesRead = ssh_scp_read(scp, buffer, sizeof(buffer))) > 0) {
            // Handle the received data
        }
    }

    ssh_scp_close(scp);
    ssh_scp_free(scp);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}
Please ensure that you have the libssh library installed and properly linked with your C project. Compile the code with the appropriate flags and libraries, including -lssh and -lssh_threads, as necessary.

Make sure to replace "hostname", "username", "password", "/path/to/destination", and "/path/to/source/file" with the actual values relevant to your scenario.

Remember to handle error cases, check return codes, and add appropriate error handling and error message printing throughout the code as per your application's requirements.
