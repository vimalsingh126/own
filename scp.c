#include <stdio.h>
#include <libssh/libssh.h>

int main()
{
    ssh_session session;
    int rc;

    session = ssh_new();
    if (session == NULL)
        exit(-1);

    ssh_options_set(session, SSH_OPTIONS_HOST, "remote-host.example.com");
    ssh_options_set(session, SSH_OPTIONS_USER, "username");
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, SSH_LOG_PROTOCOL);

    rc = ssh_connect(session);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error connecting to host: %s\n", ssh_get_error(session));
        ssh_free(session);
        exit(-1);
    }

    rc = ssh_userauth_password(session, NULL, "password");
    if (rc != SSH_AUTH_SUCCESS)
    {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(-1);
    }

    ssh_scp scp;
    scp = ssh_scp_new(session, SSH_SCP_WRITE, "/remote/path/file.txt", 0777);
    if (scp == NULL)
    {
        fprintf(stderr, "Error initializing SCP: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(-1);
    }

    rc = ssh_scp_init(scp);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error initializing SCP: %s\n", ssh_get_error(session));
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(-1);
    }

    FILE *file = fopen("/local/path/file.txt", "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening local file: %s\n", strerror(errno));
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(-1);
    }

    char buffer[1024];
    size_t len;
    while ((len = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        rc = ssh_scp_write(scp, buffer, len);
        if (rc != SSH_OK)
        {
            fprintf(stderr, "Error writing to remote file: %s\n", ssh_get_error(session));
            fclose(file);
            ssh_scp_free(scp);
            ssh_disconnect(session);
            ssh_free(session);
            exit(-1);
        }
    }

    fclose(file);
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}

