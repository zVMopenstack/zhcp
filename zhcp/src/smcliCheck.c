/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */

#include "smcliCheck.h"
#include "wrapperutils.h"

int checkAuthentication(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int passWordLength = 0;
    char * passWord = NULL;
    char * userid = NULL;
    vmApiCheckAuthenticationOutput* output;

    // Parse the command-line arguments
    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "u:p:h?")) != -1)
        switch (option) {
            case 'u':
                userid = optarg;
                break;

            case 'p':
                passWord = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Check_Authentication\n\n"
                    "SYNOPSIS\n"
                    "  smcli Check_Authentication\n\n"
                    "DESCRIPTION\n"
                    "  Use Check_Authentication to validate a userid/password pair.\n"
                    "  The following options are required:\n"
                    "    -u    User ID to validate. If the UserId is the same as the UserId of the \n"
                    "          zhcp then the password is ignored\n"
                    "    -p    Password to validate\n");
                return 1;
                break;

            default:
                return 1;
                break;
    }

    if (!userid || !passWord) {
        printf("ERROR: Missing required options\n");
        return 1;
    }
    passWordLength = strlen(passWord);
    printf("Validating userid/password pair ... ");
    rc = smCheck_Authentication(vmapiContextP, userid, passWordLength, passWord,
             &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Check_Authentication", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Check_Authentication", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}
