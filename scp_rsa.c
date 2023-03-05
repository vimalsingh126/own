#include <libssh/libssh.h>
#include <stdlib.h>

int main() {
  ssh_session session;
  ssh_scp scp;
  int port = 22;

  char* hostname = "192.168.1.100"; // Replace with the hostname or IP of the remote system
  char* username = "user"; // Replace with the username of the remote system
  char* password = NULL; // Replace with the password of the remote system, if applicable
  char* local_path = "/path/to/local/file"; // Replace with the path to the local file to transfer
  char* remote_path = "/path/to/remote/destination"; // Replace with the destination path on the remote system

  // Initialize the SSH session
  session = ssh_new();
  if (session == NULL) {
    exit(EXIT_FAILURE);
  }

  // Set the SSH options
  ssh_options_set(session, SSH_OPTIONS_HOST, hostname);
  ssh_options_set(session, SSH_OPTIONS_USER, username);
  ssh_options_set(session, SSH_OPTIONS_PORT, &port);
  ssh_options_set(session, SSH_OPTIONS_RSAKEY, "/path/to/rsa/key"); // Replace with the path to the RSA key

  // Connect to the SSH server
  int rc = ssh_connect(session);
  if (rc != SSH_OK) {
    ssh_free(session);
    exit(EXIT_FAILURE);
  }

  // Authenticate the session
  if (ssh_userauth_publickey_auto(session, NULL, NULL) != SSH_AUTH_SUCCESS) {
    ssh_disconnect(session);
    ssh_free(session);
    exit(EXIT_FAILURE);
  }

  // Initialize the SCP transfer
  scp = ssh_scp_new(session, SSH_SCP_WRITE, remote_path);
  if (scp == NULL) {
    ssh_disconnect(session);
    ssh_free(session);
    exit(EXIT_FAILURE);
  }

  // Start the SCP transfer
  rc = ssh_scp_init(scp);
  if (rc != SSH_OK) {
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    ssh_disconnect(session);
    ssh_free(session);
    exit(EXIT_FAILURE);
  }

  // Add the local file to the SCP transfer
  rc = ssh_scp_push_file(scp, local_path, 0);
  if (rc != SSH_OK) {
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    ssh_disconnect(session);
    ssh_free(session);
    exit(EXIT_FAILURE);
  }

  // Close the SCP transfer
  ssh_scp_close(scp);
  ssh_scp_free(scp);

  // Disconnect from the SSH server and free the session
  ssh_disconnect(session);
  ssh_free(session);

  exit(EXIT_SUCCESS);
}
