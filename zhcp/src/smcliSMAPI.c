/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */

#include "smcliSMAPI.h"
#include "wrapperutils.h"

int smapiStatusCapture(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * profile = NULL;
    vmApiSMAPIStatusCaptureOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                profile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  SMAPI_Status_Capture\n\n"
                    "SYNOPSIS\n"
                    "  smcli SMAPI_Status_Capture [-T] target_name\n\n"
                    "DESCRIPTION\n"
                    "  Use SMAPI_Status_Capture to capture data to assist with identification and\n"
                    "  resolution of a problem with the SMAPI servers.\n"
                    "  The following options are required:\n"
                    "    -T    The name of the target used for authorization.\n");
                return 1;
                break;

            case '?':  // Missing option data!
                return 1;
                  break;

            case 1:  // API name type data(other non option element key data)
                break;

            default:
                return 1;
                break;
        }

    if (!profile) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    printf("Checking SMAPI status... ");
    rc = smSMAPI_Status_Capture(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            profile, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("SMAPI_Status_Capture", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
    	rc = printAndLogSmapiReturnCodeReasonCodeDescription("SMAPI_Status_Capture", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    
    return rc;
}
