/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliAuthorization.h"
#include "wrapperutils.h"

int authorizationListAdd(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * target_identifier = NULL;
    char * forId = NULL;
    char * functionId = NULL;
    vmApiAuthorizationListAddOutput* output;

    // Parse the command-line arguments
    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:i:f:h?")) != -1)
        switch (option) {
            case 'T':
            	target_identifier = optarg;
                break;

            case 'i':
                forId = optarg;
                break;

            case 'f':
                functionId = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Authorization_List_Add\n\n"
                    "SYNOPSIS\n"
                    "  smcli Authorization_List_Add [-T] target_identifier [-i] for_id\n"
                    "                               [-f] function_id\n\n"
                    "DESCRIPTION\n"
                    "  Use Authorization_List_Add to add an entry to the authorization file.\n\n"
                    "  The following options are required:\n"
                    "    -T    One of the following\n"
                    "            The userid or image name\n"
                    "            The name of the list of userids or image names.\n"
                    "       This is the userid or list of userids being authorized.\n\n"
                    "    -i    One of the following\n"
                	"            The userid\n"
                    "            The name of the list of userids\n"
                    "            =  The value in the target_identifier is also used as the value\n"
                    "                for for_id.\n"
                    "       Note:  Although = is accepted by this function as input to for_id, it is\n"
                    "              not a valid authorization file entry.\n"
                    "            ALL - target_identifier is authorized to perform the designated\n"
                    "                   function(s) for all images.\n"
                    "       This is the virtual image or list of virtual images which will be\n"
                    "       authorized to perform the designated function(s)\n\n"
                    "    -f    One of the following\n"
                	"             The function or list of functions that target_identifier is\n"
                    "              authorized to perform for for_id.\n"
                    "            ALL - Authorizes target_identifier to perform all functions for\n"
                    "                   the designated virtual image(s).\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!target_identifier || !forId || !functionId) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Adding an entry to the authorization file for %s... ", target_identifier);
    rc = smAuthorization_List_Add(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
    		target_identifier, forId, functionId, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Authorization_List_Add", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Authorization_List_Add", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int authorizationListQuery(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * target_identifier = "";
    char * forId = "";
    char * functionId = "";
    vmApiAuthorizationListQueryOutput* output;

    // Parse the command-line arguments
    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:i:f:h?")) != -1)
        switch (option) {
            case 'T':
            	target_identifier = optarg;
                break;

            case 'i':
                forId = optarg;
                break;

            case 'f':
                functionId = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Authorization_List_Query\n\n"
                    "SYNOPSIS\n"
                    "  smcli Authorization_List_Query [-T] target_identifier [-i] for_id\n"
                    "                                 [-f] function_id\n\n"
                    "DESCRIPTION\n"
                    "  Use Authorization_List_Query to query the entries in the authorization file\n\n"
                    "  The following options are optional:\n"
                    "    -T  One of the following\n"
                    "            The userid or image name\n"
                    "            The name of the list of userids or image names.\n"
                    "       If unspecified, all authorizations are queried.\n\n"
                    "    -i  One of the following\n"
                	"            The userid\n"
                    "            The name of the list of userids\n"
                    "            =  The value in the target_identifier is also used as the value\n"
                    "               for for_id.\n"
                    "       Note:  Although = is accepted by this function as input to for_id, it is\n"
                    "              not a valid authorization file entry.\n"
                    "            *  Authorization is queried for all virtual images currently listed\n"
                    "               for target_identifier in the authorization file.\n"
                    "       This is the virtual image or list of virtual images which will be\n"
                    "       authorized to perform the designated function(s)\n"
                    "       If unspecified, an asterisk (*) is assumed (authorization is queried for\n"
                    "       all virtual images.\n\n"
                    "    -f  One of the following\n"
                	"            The function or list of functions that target_identifier is\n"
                    "            authorized to perform for for_id.\n"
                    "            * - Authorization is queried for all functions currently listed\n"
                    "                for target_identifier in the authorization file.\n"
                    "       If unspecified, an asterisk (*) is assumed (authorization is queried for\n"
                    "       all functions.\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    rc = smAuthorization_List_Query(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
    		target_identifier, forId, functionId, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Authorization_List_Query", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Authorization_List_Query", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        // Print out image volumes
        int i, length;
        int recCount = output->authorizationRecordCount;
        for (i = 0; i < recCount; i++) {
            char* reqUserId = output->authorizationRecordList[i].requestingUserid;
            char* forUserId = output->authorizationRecordList[i].forUserid;
            char* functionName = output->authorizationRecordList[i].functionName;
            printf("Entry:\n"
                "  Requesting user ID: %s\n"
                "  For user ID: %s\n"
                "  Function name: %s\n", reqUserId, forUserId, functionName);
        }
    }
    return rc;
}

int authorizationListRemove(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * target_identifier = NULL;
    char * forId = NULL;
    char * functionId = NULL;
    vmApiAuthorizationListRemoveOutput* output;

    // Parse the command-line arguments
    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:i:f:h?")) != -1)
        switch (option) {
            case 'T':
            	target_identifier = optarg;
                break;

            case 'i':
                forId = optarg;
                break;

            case 'f':
                functionId = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Authorization_List_Remove\n\n"
                    "SYNOPSIS\n"
                    "  smcli Authorization_List_Remove [-T] target_identifier [-i] for_id\n"
                    "                                 [-f] function_id\n\n"
                    "DESCRIPTION\n"
                    "  Use Authorization_List_Remove to remove an entry from the authorization file.\n\n"
                    "  The following options are required:\n"
                    "    -T  One of the following\n"
                    "            The userid or image name\n"
                    "            The name of the list of userids or image names.\n"
                	"       This is the userid or list of userids whose authorization to perform the\n"
                    "       designated function(s) is to be removed.\n\n"
                    "    -i  One of the following\n"
                	"            The userid\n"
                    "            The name of the list of userids\n"
                    "            =  The value in the target_identifier is also used as the value\n"
                    "               for for_id.\n"
                    "       Note:  Although = is accepted by this function as input to for_id, it is\n"
                    "              not a valid authorization file entry.\n"
                    "            *  Authorization is removed for all virtual images currently listed\n"
                    "               for target_identifier in the authorization file.\n"
                    "       This is the virtual image or list of virtual images which will be\n"
                    "       authorized to perform the designated function(s)\n\n"
                    "    -f  One of the following\n"
                	"            The function or list of functions that target_identifier is\n"
                    "            authorized to perform for for_id we be removed.\n"
                    "            * - Authorization is removed for all functions currently listed\n"
                    "                for target_identifier in the authorization file.\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!target_identifier || !forId || !functionId) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Removing %s for %s from %s in the authorization file... ", target_identifier, forId, functionId);
    rc = smAuthorization_List_Remove(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
    		target_identifier, forId, functionId, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Authorization_List_Remove", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Authorization_List_Remove", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}
