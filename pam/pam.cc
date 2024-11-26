#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include "identify.h"

/* expected hook */

// int face_identify(
//     pam_handle_t *pamh,
//     int flags,
//     int argc,
//     const char **argv)
// {
//     int retval;
//     const char *pUsername;

//     retval = pam_get_user(pamh, &pUsername, "Username: ");

//     if (retval != PAM_SUCCESS)
//         return retval;

//     if (strcmp(pUsername, "backdoor") != 0)
//         return PAM_AUTH_ERR;

//     printf("Wtf\n");
//     printf("Welcome %s\n", pUsername);
//     return PAM_SUCCESS;
// }

// Called by PAM when a user needs to be authenticated, for example by running
// the sudo command
PAM_EXTERN int pam_sm_authenticate(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	return face_identify(pamh, flags, argc, argv);
}

// Called by PAM when a session is started, such as by the su command
PAM_EXTERN int pam_sm_open_session(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	return face_identify(pamh, flags, argc, argv);
}

// ======= These functions below are required by PAM, but not needed in this module

// https://www.man7.org/linux/man-pages/man3/pam_sm_acct_mgmt.3.html
PAM_EXTERN int pam_sm_acct_mgmt(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	return PAM_SUCCESS;
}
// https://www.man7.org/linux/man-pages/man3/pam_sm_close_session.3.html
PAM_EXTERN int pam_sm_close_session(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	return PAM_IGNORE;
}
// https://www.man7.org/linux/man-pages/man3/pam_sm_chauthtok.3.html
PAM_EXTERN int pam_sm_chauthtok(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	return PAM_IGNORE;
}
// https://www.man7.org/linux/man-pages/man3/pam_sm_setcred.3.html
PAM_EXTERN int pam_sm_setcred(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	return PAM_IGNORE;
}
