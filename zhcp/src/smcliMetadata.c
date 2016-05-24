/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliMetadata.h"
#include "wrapperutils.h"

int metadataDelete(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    // Parse the command-line arguments
    int option;
    int rc;
    char * image = NULL;
    int i;
    const char * optString = "-T:k:h?";
    char * keywords = 0;
    vmApiMetadataDeleteOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, optString)) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'k':
                keywords = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Metadata_Delete\n\n"
                    "SYNOPSIS\n"
                    "  smcli Metadata_Delete [-T] image_name\n"
                    "    [-k] 'metadata_name1 metadata_name2 ...'\n\n"
                    "DESCRIPTION\n"
                    "  Use Metadata_Delete to delete metadata values associated with\n"
                    "  a textual identifier (typically a directory entry name)..\n"
                    "  The following options are required:\n"
                    "    -T    A textual identifier (typically a directory entry name).\n"
                    "    -k    A blank-delimited list of metadata names. \n"
                    "          Note that these metadata names are case sensitive.\n");
                return 1;
                break;

            case '?':  // Missing option data or invalid option
                return 1;
                break;

            case 1:  // API name type data(other non option element key data)
                break;

            default:
                return 1;
                break;
        }

    if (!image || !keywords) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Deleting metadata values associated with %s... ", image);
    rc = smMetadataDelete(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            image, keywords, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("MetadataDelete", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("MetadataDelete", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int metadataGet(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    // Parse the command-line arguments
    int option;
    int rc;
    char * image = NULL;
    int i;
    const char * optString = "-T:k:h?";
    char * keywords = NULL;
    vmApiMetadataGetOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, optString)) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'k':
                keywords = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Metadata_Get\n\n"
                    "SYNOPSIS\n"
                    "  smcli Metadata_Get [-T] image_name\n"
                    "    [-k] 'metadata1 metadata2 ...'\n\n"
                    "DESCRIPTION\n"
                    "  Use Metadata_Get to obtain metadata values associated with a\n"
                    "  textual identifier (typically a directory entry name).\n"
                    "  The following options are required:\n"
                    "    -T    A textual identifier (typically a directory entry name).\n"
                    "    -k    A quoted blank-delimited list of metadata names.\n"
                    "          Note that these metadata names are case sensitive.\n");
                return 1;
                break;

            case '?':  // Missing option data or invalid option
                return 1;
                break;

            case 1:  // API name type data(other non option element key data)
               break;

            default:
                return 1;
                break;
        }

    if (!image || !keywords) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    rc = smMetadataGet(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            image, keywords, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("MetadataGet", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("MetadataGet", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        int i;
        int entryCount = output->metadataEntryCount;
        if (entryCount > 0) {
            // Loop through metadata entries and print out metadata entry
            for (i = 0; i < entryCount; i++) {
                if ((output->metadataEntryList[i].metadataEntryName != NULL) &&
                    (output->metadataEntryList[i].metadata != NULL)) {
                   printf("%s:%s\n", output->metadataEntryList[i].metadataEntryName,
                                     output->metadataEntryList[i].metadata);
                }
            }
        }
    }
    return rc;
}

int metadataSet(int argC, char* argV[],
        struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int option;
    int entryCount = 0;
    int metadataStructureLength = 0;
    char * token;
    char * buffer;
    char * targetIdentifier = NULL;
    char ** entryArray;
    int argBytes = 0;

    vmApiMetadataSetOutput* output;
    const char * optString = "-T:k:h?";

    // Count up the max number of arguments to create the array
    opterr = 0;  // Suppress messages
    while ((option = getopt(argC, argV, optString)) != -1) {
        argBytes = argBytes + sizeof(*entryArray);
    }

    optind = 1;  // Reset optind so getopt can rescan arguments
    opterr = 1;  // Enable messages

    if (argBytes > 0) {
        if (0 == (entryArray = malloc(argBytes)))
            return MEMORY_ERROR;
    }

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, optString)) != -1)
        switch (option) {
            case 'T':
            	targetIdentifier = optarg;
                break;

            case 'k':
                entryArray[entryCount] = optarg;
                entryCount++;
                break;

            case 'h':
                printf("NAME\n"
                    "  Metadata_Set\n\n"
                    "SYNOPSIS\n"
                    "  smcli Metadata_Set [-T] image_name\n"
                    "    [-k] 'metadata1 metadata2 ...'\n\n"
                    "DESCRIPTION\n"
                    "  Use Metadata_Set to set metadata values associated with a\n"
                    "  textual identifier (typically a directory entry name).\n"
                    "  The following options are required:\n"
                    "    -T    A textual identifier (typically a directory entry name).\n"
                    "    -k     One or more quoted metadata_name=metadata item to be created\n"
                    "           The keyword is a unique value for this identifier\n"
                    "          Note that these metadata_names are case sensitive.\n");
                FREE_MEMORY_CLEAR_POINTER(entryArray);
                return 1;
                break;

            case 1:  // API name type data(other non option element key data)
                break;

            case '?':   // Missing option data or invalid option
                FREE_MEMORY_CLEAR_POINTER(entryArray);
                return 1;
                break;

            default:
                FREE_MEMORY_CLEAR_POINTER(entryArray);
                return 1;
                break;
        }

    if (!targetIdentifier || !entryCount) {
        printf("\nERROR: Missing required options\n");
        if (argBytes > 0) {
            FREE_MEMORY_CLEAR_POINTER(entryArray);
        }
        return 1;
    }

    char *nameArray[entryCount];
    char *dataArray[entryCount];
    for (i = 0; i < entryCount; i ++) {
        token = strtok_r(entryArray[i], "=", &buffer);
        if (token != NULL) {
            nameArray[i] = token;
        } else {
            printf("ERROR: metadata_entry_name is NULL\n");
            FREE_MEMORY_CLEAR_POINTER(entryArray);
            return 1;
        }
        token = strtok_r(NULL, "\0", &buffer);
        if (token != NULL) {
            dataArray[i] = token;
        } else {
            printf("ERROR: metadata is NULL\n");
            FREE_MEMORY_CLEAR_POINTER(entryArray);
            return 1;
        }
    }

    printf("Setting metadata values associated with %s... ", targetIdentifier);
    rc = smMetadataSet(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
    		targetIdentifier, entryCount, nameArray, dataArray, &output);
    FREE_MEMORY_CLEAR_POINTER(entryArray);

    if (rc) {
        printAndLogSmapiCallReturnCode("MetadataSet", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("MetadataSet", output->common.returnCode,
               output->common.reasonCode, vmapiContextP);
    };
    return rc;
}
