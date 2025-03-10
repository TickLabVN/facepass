#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include "identify.h"

// Called by PAM when a user needs to be authenticated, for example by running
// the sudo command
PAM_EXTERN int pam_sm_authenticate(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	int retval;
	const char* pUsername;
	retval = pam_get_user(pamh, &pUsername, "Username: ");

	printf("Welcome %s\n", pUsername);
	if (retval != PAM_SUCCESS)
		return retval;

	retval = face_identify(pUsername);
	if (retval == PAM_SUCCESS)
		printf("Face recognized.\n");
	else
		printf("Face not recognized\n");
	return retval;
}

// Called by PAM when a session is started, such as by the su command
PAM_EXTERN int pam_sm_open_session(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	return PAM_IGNORE;
}

// ======= These functions below are required by PAM, but not needed in this module
// https://www.man7.org/linux/man-pages/man3/pam_sm_acct_mgmt.3.html
PAM_EXTERN int pam_sm_acct_mgmt(
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
	printf("pam_sm_setcred\n");
	return PAM_SUCCESS;
}
