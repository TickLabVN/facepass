#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include "face_config.h"

const struct pam_conv conv = {
	misc_conv,
	NULL};

int main(int argc, char *argv[])
{
	pam_handle_t *pamh = NULL;
	int retval;

	auto users = get_usernames();

	for (const auto& user : users)
	{
		retval = pam_start("check_user", user.c_str(), &conv, &pamh);

		// Are the credentials correct?
		if (retval == PAM_SUCCESS)
			retval = pam_authenticate(pamh, 0);

		// Can the account be used at this time?
		if (retval == PAM_SUCCESS)
		{
			printf("Account %s is valid.\n", user.c_str());
			retval = pam_acct_mgmt(pamh, 0);
		}

		// Did everything work?
		if (retval == PAM_SUCCESS) {
			printf("User %s authenticated\n", user.c_str());
			break;
		} else {
			printf("User %s not authenticated\n", user.c_str());
		}

		// close PAM (end session)
		if (pam_end(pamh, retval) != PAM_SUCCESS)
		{
			pamh = NULL;
			printf("check_user: failed to release authenticator for user %s\n", user.c_str());
			exit(1);
		}
	}
	return 0;
}