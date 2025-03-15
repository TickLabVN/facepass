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
	// return PAM_SUCCESS;
	int retval;
	const char *pUsername;
	retval = pam_get_user(pamh, &pUsername, NULL);

	if (retval != PAM_SUCCESS)
		return retval;

	retval = face_identify(pUsername);
	if (retval == PAM_SUCCESS)
		printf("Face recognized! Welcome %s\n", pUsername);
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
	// return PAM_SUCCESS;
	const char *pUsername;
	int retval = pam_get_user(pamh, &pUsername, NULL);
	if (retval != PAM_SUCCESS)
		return retval;
	return face_identify(pUsername);
}

// The functions below are required by PAM, but not needed in this module
PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc,
								const char **argv)
{
	return PAM_IGNORE;
}
PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc,
									const char **argv)
{
	return PAM_IGNORE;
}
PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc,
								const char **argv)
{
	return PAM_IGNORE;
}
PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc,
							  const char **argv)
{
	return PAM_IGNORE;
}