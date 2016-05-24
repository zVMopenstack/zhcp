/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliResponse.h"
#include "wrapperutils.h"

int responseRecovery(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int failedRequestID = 0;
    char * image = NULL;
    vmApiResponseRecoveryOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:f:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'f':
                failedRequestID = atoi(optarg);
                break;

            case 'h':
                printf("NAME\n"
                    "  Response_Recovery\n\n"
                    "SYNOPSIS\n"
                    "  smcli Response_Recovery [-T] image_name [-f] failedRequestID\n\n"
                    "DESCRIPTION\n"
                    "  Use Response_Recovery to obtain response data from previous calls that may\n"
                    "  have failed.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n"
                    "    -f    Previously failed requestId for which you wish to recover\n"
                    "          response data\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image || !failedRequestID) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smResponse_Recovery(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            image, failedRequestID, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Response_Recovery", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Response_Recovery", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        printf("Response Data for %s: %s", image, output->responseData->vmapiString);
    }
    return rc;
}
