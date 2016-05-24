/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliName.h"
#include "wrapperutils.h"

int nameListAdd(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * nameList = NULL;
    char * name = NULL;
    vmApiNameListAddOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:n:h?")) != -1)
        switch (option) {
            case 'T':
                nameList = optarg;
                break;

            case 'n':
                name = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Name_List_Add\n\n"
                    "SYNOPSIS\n"
                    "  smcli Name_List_Add [-T] name_list [-n] name\n\n"
                    "DESCRIPTION\n"
                    "  Use Name_List_Add to add a name to a list in the name list file.\n"
                    "  If the list that is specified in target_identifier does not exist,\n"
                    "  a new list will be created.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the list that is being updated\n"
                    "    -n    The name to be added to the list specified\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!nameList || !name) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    printf("Adding %s to %s... ", name, nameList);
    rc = smName_List_Add(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
            nameList, name, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Name_List_Add", rc, vmapiContextP);
    } else if ( (output->common.returnCode == 0) && (output->common.reasonCode == 12) ) {
        // Handle the special case where the first Name is added and thus a new Name List is created
        printf("Done. New list created. \n");
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Name_List_Add", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int nameListDestroy(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * nameList = NULL;
    vmApiNameListDestroyOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                nameList = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Name_List_Destroy\n\n"
                    "SYNOPSIS\n"
                    "  smcli Name_List_Destroy [-T] name_list\n\n"
                    "DESCRIPTION\n"
                    "  Use Name_List_Destroy to destroy a list from the name list file.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the list being destroyed\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!nameList) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    printf("Destroying %s... ", nameList);
    rc = smName_List_Destroy(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
            nameList, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Name_List_Destroy", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Name_List_Destroy", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int nameListQuery(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int i;
    char * nameList = NULL;
    vmApiNameListQueryOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                nameList = optarg;
                break;

        case 'h':
                printf("NAME\n"
                    "  Name_List_Query\n\n"
                    "SYNOPSIS\n"
                    "  smcli Name_List_Query [-T] name_list\n\n"
                    "DESCRIPTION\n"
                    "  Use Name_List_Query to query the names that are in a list in the\n"
                    "  name list file.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the list being queried\n");
                return 1;
                break;

            default:
                return 1;
                break;
    }

    if (!nameList) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smName_List_Query(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
            nameList, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Name_List_Query", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Name_List_Query", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        // Print out the Names in the nameList
        for (i = 0; i < output->nameArrayCount; i++) {
            printf("%s\n", output->nameList[i]);
        }
    }
    return rc;
}

int nameListRemove(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * nameList = NULL;
    char * name = NULL;
    vmApiNameListRemoveOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:n:h?")) != -1)
        switch (option) {
            case 'T':
                nameList = optarg;
                break;

            case 'n':
                name = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Name_List_Remove\n\n"
                    "SYNOPSIS\n"
                    "  smcli Name_List_Remove [-T] name_list [-n] name\n\n"
                    "DESCRIPTION\n"
                    "  Use Name_List_Remove to delete a name from a list in the name list\n"
                    "  file. If there are no names remaining in the list, the list is also\n"
                    "  deleted.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the list that is being updated\n"
                    "    -n    A userid or function name or list\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!nameList || !name) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    printf("Deleting %s from %s... ", name, nameList);
    rc = smName_List_Remove(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
             nameList, name, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Name_List_Remove", rc, vmapiContextP);
    } else if ( (output->common.returnCode == 0) && (output->common.reasonCode == 16) ) {  
        // Handle the special case where the last Name is removed and thus the Name List is destroyed
        printf("Done. No more entries, list destroyed. \n");
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Name_List_Remove", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}


