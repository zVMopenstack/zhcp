/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliDirectory.h"
#include "wrapperutils.h"

int directoryManagerLocalTagDefineDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int tagOrdinal = 0;
    int action = 0;
    char * image = NULL;
    char * tagName = NULL;
    vmApiDirectoryManagerLocalTagDefineDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:n:o:a:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'n':
                tagName = optarg;
                break;

            case 'o':
                tagOrdinal = atoi(optarg);
                break;

            case 'a':
                action = atoi(optarg);
                break;

            case 'h':
                printf("NAME\n"
                    "  Directory_Manager_Local_Tag_Define_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Directory_Manager_Local_Tag_Define_DM [-T] image_name\n"
                    "  [-t] tag_name [-o] tag_ordinal [-a] action\n\n"
                    "DESCRIPTION\n"
                    "  Use Directory_Manager_Local_Tag_Define_DM to define a local tag or named\n"
                    "  comment record to contain installation-specific information about a virtual\n"
                    "  image.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n"
                    "    -n    The name of the local tag or named comment to be defined\n"
                    "    -o    The value of the tag sort ordinal, relative to other defined\n"
                    "          local tags\n"
                    "    -a    Specifies creation of a new tag or change of a tag ordinal value:\n"
                    "            1: Create a new tag\n"
                    "            2: Change an existing tag's ordinal value\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }


    if (!image || !tagName || !tagOrdinal || !action) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Defining a local tag or named comment record %s... ", tagName);
    rc = smDirectory_Manager_Local_Tag_Define_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
             image, tagName, tagOrdinal, action, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Directory_Manager_Local_Tag_Define_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Directory_Manager_Local_Tag_Define_DM",
                output->common.returnCode, output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int directoryManagerLocalTagDeleteDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    char * tagName = NULL;
    vmApiDirectoryManagerLocalTagDeleteDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:n:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'n':
                tagName = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Directory_Manager_Local_Tag_Delete_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Directory_Manager_Local_Tag_Delete_DM [-T] image_name [-n] tag_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Directory_Manager_Local_Tag_Delete_DM to remove a local tag or\n"
                    "  named comment record from the directory manager's internal tables.\n"
                    "  Users will no longer be able to set or query the tag.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file that\n"
                    "          also contains the authenticated_userid and the function_name\n"
                    "    -n    Specifies the name of the tag to be deleted\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image || !tagName) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Removing a local tag or named comment record %s... ", tagName);
    rc = smDirectory_Manager_Local_Tag_Delete_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
             image, tagName, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Directory_Manager_Local_Tag_Delete_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Directory_Manager_Local_Tag_Delete_DM",
                output->common.returnCode, output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int directoryManagerLocalTagQueryDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    char * tagName = NULL;
    vmApiDirectoryManagerLocalTagQueryDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:n:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'n':
                tagName = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Directory_Manager_Local_Tag_Query_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Directory_Manager_Local_Tag_Query_DM [-T] image_name [n] tag_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Directory_Manager_Local_Tag_Query_DM to obtain the value of a\n"
                    "  virtual image's local tag or named comment record.\n\n"
                    "  The following options are required:\n"
                    "    -T    The target userid whose tag is being queried\n"
                    "    -n    The name of the local tag or named comment to be queried\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image || !tagName) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    rc = smDirectory_Manager_Local_Tag_Query_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
             image, tagName, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Directory_Manager_Local_Tag_Query_DM", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Directory_Manager_Local_Tag_Query_DM",
                output->common.returnCode, output->common.reasonCode, vmapiContextP);
    }  else {
        // Print value of the associated tag
        printf("%s\n", output->tagValue);
    }
    return rc;
}

int directoryManagerLocalTagSetDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    char * tagName = NULL;
    char * tagValue = NULL;
    vmApiDirectoryManagerLocalTagSetDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:n:v:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'n':
                tagName = optarg;
                break;

            case 'v':
                tagValue = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Directory_Manager_Local_Tag_Set_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Directory_Manager_Local_Tag_Set_DM [-T] image_name\n"
                    "  [-n] tag_name [-v] tag_value\n\n"
                    "DESCRIPTION\n"
                    "  Use Directory_Manager_Local_Tag_Set_DM to set the value of a virtual\n"
                    "  image's local tag or named comment record.\n\n"
                    "  The following options are required:\n"
                    "    -T    The target userid whose tag is being set\n"
                    "    -n    The name of the local tag or named comment to be set\n"
                    "    -v    The value of a virtual image's local tag or named comment\n"
                    "          to be set\n");
                return 1;
                break;

            default:
                return 1;
                break;
         }


    if (!image || !tagName || !tagValue) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Setting the value of a virtual image's local tag or named comment record %s... ", tagName);
    rc = smDirectory_Manager_Local_Tag_Set_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
             image, tagName, strlen(tagValue), tagValue, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Directory_Manager_Local_Tag_Set_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Directory_Manager_Local_Tag_Set_DM",
                output->common.returnCode, output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int directoryManagerSearchDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int i;
    int j;
    int recCount;
    int statementLength;
    char targetId[8 + 1];
    char statement[72 + 1];
    char * image = NULL;
    char * searchPattern = NULL;
    vmApiDirectoryManagerSearchDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:s:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 's':
                searchPattern = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Directory_Manager_Search_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Directory_Manager_Search_DM [-T] image_name [-s] search_pattern\n\n"
                    "DESCRIPTION\n"
                    "  Use Directory_Manager_Search_DM to search the directory for records that\n"
                    "  match the specified pattern.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n"
                    "    -s    The records to be searched for. Tokens must be separated by blanks\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image || !searchPattern) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smDirectory_Manager_Search_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
             image, strlen(searchPattern), searchPattern, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Directory_Manager_Search_DM", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Directory_Manager_Search_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        // Print out image names
        recCount = output->statementCount;
        if (recCount > 0) {
            // Loop through the records and print it out
            for (i = 0; i < recCount; i++) {
                printf("%s: ", output->statementList[i].targetId);
                for (j = 0; j < output->statementList[i].statementLength; j++){
                    encode_print(output->statementList[i].statement[j]);
                }
                printf("\n");
            }
        }
    }
    return rc;
}

int directoryManagerTaskCancelDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int opId = 0;
    char * image = NULL;
    vmApiDirectoryManagerTaskCancelDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:i:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'i':
               opId = atoi(optarg);
               break;

            case 'h':
                printf("NAME\n"
                    "  Directory_Manager_Task_Cancel_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Directory_Manager_Task_Cancel_DM [-T] image_name\n"
                    "  [-i] operation_id\n\n"
                    "DESCRIPTION\n"
                    "  Use Directory_Manager_Task_Cancel_DM to cancel a specific asynchronous\n"
                    "  task being performed by the directory manager.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n"
                    "    -i    The identifier of the task\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }


    if (!image || !opId) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Canceling asynchronous task %d... ", opId);
    rc = smDirectory_Manager_Task_Cancel_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
            image, opId, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Directory_Manager_Task_Cancel_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Directory_Manager_Task_Cancel_DM",
                output->common.returnCode, output->common.reasonCode, vmapiContextP);
    }
    return rc;
}
