#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <stdio.h>

int main()
{
    ssh_session ssh = ssh_new();
    ssh_options_set(ssh, SSH_OPTIONS_HOST, "192.168.1.10"); // replace with target IP address
    ssh_options_set(ssh, SSH_OPTIONS_USER, "username"); // replace with target username

    int rc = ssh_connect(ssh);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to remote host: %s\n", ssh_get_error(ssh));
        ssh_free(ssh);
        return 1;
    }

    rc = ssh_userauth_password(ssh, NULL, "password"); // replace with target password
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with remote host: %s\n", ssh_get_error(ssh));
        ssh_disconnect(ssh);
        ssh_free(ssh);
        return 1;
    }

    ssh_scp scp = ssh_scp_new(ssh, SSH_SCP_WRITE, "/home/username/testfile.txt"); // replace with target file path
    if (scp == NULL) {
        fprintf(stderr, "Error initializing SCP transfer: %s\n", ssh_get_error(ssh));
        ssh_disconnect(ssh);
        ssh_free(ssh);
        return 1;
    }

    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error initializing SCP transfer: %s\n", ssh_get_error(ssh));
        ssh_scp_free(scp);
        ssh_disconnect(ssh);
        ssh_free(ssh);
        return 1;
    }

    FILE *fp = fopen("testfile.txt", "r"); // replace with source file path
    if (fp == NULL) {
        fprintf(stderr, "Error opening source file: %s\n", strerror(errno));
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        ssh_disconnect(ssh);
        ssh_free(ssh);
        return 1;
    }

    char buffer[4096];
    size_t len;
    while ((len = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        rc = ssh_scp_write(scp, buffer, len);
        if (rc != SSH_OK) {
            fprintf(stderr, "Error writing to remote host: %s\n", ssh_get_error(ssh));
            fclose(fp);
            ssh_scp_close(scp);
            ssh_scp_free(scp);
            ssh_disconnect(ssh);
            ssh_free(ssh);
            return 1;
        }
    }

    fclose(fp);
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    ssh_disconnect(ssh);
    ssh_free(ssh);

    printf("File transfer completed successfully.\n");

    return 0;
}
