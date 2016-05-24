/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliPrototype.h"
#include "wrapperutils.h"


int prototypeCreateDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int recordCount = 0;
    int c;
    int option;
    char * prototype = NULL;
    char * file = NULL;
    FILE * fp = NULL;
    vmApiPrototypeCreateDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:f:h?")) != -1)
        switch (option) {
            case 'T':
                prototype = optarg;
                break;

            case 'f':
                file = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Prototype_Create_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Prototype_Create_DM [-T] prototype_name [-f] prototype_file\n\n"
                    "DESCRIPTION\n"
                    "  Use Prototype_Create_DM to create a new virtual image prototype.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the image being activated\n"
                    "    -f    The new virtual image prototype");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!prototype || !file) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    // Open the user entry file
    fp = fopen(file, "r");
    if (fp != NULL) {
        // Count the number of lines and set the record count to it
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {
                recordCount++;
            }
        }
    } else {
        printf("\nERROR: Failed to open file %s\n", file);
        return 1;
    }
    // Reset position to start of file
    rewind(fp);

    // Create prototype record
    vmApiPrototypeRecordList record[recordCount];
    int i = 0, LINE_SIZE = 72;
    char line[recordCount][LINE_SIZE];
    char * ptr;
    while (fgets(line[i], LINE_SIZE, fp) != NULL) {
        // Replace newline with null terminator
        ptr = strstr(line[i], "\n");
        if (ptr != NULL) {
            strncpy(ptr, "\0", 1);
        }
        record[i].recordNameLength = strlen(line[i]);
        record[i].recordName = line[i];
        i++;
    }

    // Close file
    fclose(fp);

    printf("Creating new image prototype %s... ", prototype);
    rc = smPrototype_Create_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            prototype, recordCount, (vmApiPrototypeRecordList *) record, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Prototype_Create_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Prototype_Create_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int prototypeDeleteDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * prototype = NULL;
    vmApiPrototypeDeleteDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                prototype = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Prototype_Delete_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Prototype_Delete_DM [-T] prototype_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Prototype_Delete_DM to delete an image prototype.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the prototype to be deleted\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!prototype) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    printf("Deleting image prototype %s... ", prototype);
    rc = smPrototype_Delete_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            prototype, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Prototype_Delete_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Prototype_Delete_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int prototypeNameQueryDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * prototype = NULL;
    vmApiPrototypeNameQueryDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                prototype = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Prototype_Name_Query_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Prototype_Name_Query_DM [-T] prototype_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Prototype_Name_Query_DM to obtain a list of names of defined prototypes.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!prototype) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smPrototype_Name_Query_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            prototype,  // Does not matter what image name is used
            &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Prototype_Name_Query_DM", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Prototype_Name_Query_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        // Print out image names
        int i;
        int n = output->nameArrayCount;
        for (i = 0; i < n; i++) {
            printf("%s\n", output->nameList[i]);
        }
    }
    return rc;
}

int prototypeQueryDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int option;
    int recLen;
    int recCount;
    char line[72];
    char * prototype = NULL;
    vmApiPrototypeQueryDmOutput * output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                prototype = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Prototype_Query_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Prototype_Query_DM [-T] prototype_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Prototype_Query_DM to query the characteristics of an image prototype.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the prototype to be queried\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!prototype) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smPrototype_Query_DM(vmapiContextP, "", 0, "", prototype, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Prototype_Query_DM", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Prototype_Query_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        // Print out directory entry
        recCount = output->recordArrayCount;
        if (recCount > 0) {
            for (i = 0; i < recCount; i++) {
                recLen = output->recordList[i].recordNameLength;
                memset(line, 0, recLen);
                memcpy(line, output->recordList[i].recordName, recLen);
                line[recLen] = '\0';
                printf("%s\n", line);
            }
        }
    }
    return rc;
}

int prototypeReplaceDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int recordCount = 0;
    int c;
    char * prototype = NULL;
    char * file = NULL;
    FILE * fp = NULL;
    vmApiPrototypeReplaceDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:f:h?")) != -1)
        switch (option) {
            case 'T':
                prototype = optarg;
                break;

            case 'f':
                file = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Prototype_Replace_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Prototype_Replace_DM [-T] prototype_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Prototype_Replace_DM to replace an existing prototype.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the image being activated\n"
                    "    -f    The new virtual image prototype\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!prototype || !file) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    // Open the user entry file
    fp = fopen(file, "r");
    if (fp != NULL) {
        // Count the number of lines and set the record count to it
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {
                recordCount++;
            }
        }
    } else {
        printf("\nERROR: Failed to open file %s\n", file);
        return 1;
    }
    // Reset position to start of file
    rewind(fp);

    // Create prototype record
    vmApiPrototypeRecordList record[recordCount];
    int i = 0, LINE_SIZE = 72;
    char line[recordCount][LINE_SIZE];
    char * ptr;
    while (fgets(line[i], LINE_SIZE, fp) != NULL) {
        // Replace newline with null terminator
        ptr = strstr(line[i], "\n");
        if (ptr != NULL) {
            strncpy(ptr, "\0", 1);
        }
        record[i].recordNameLength = strlen(line[i]);
        record[i].recordName = line[i];
        i++;
    }

    // Close file
    fclose(fp);

    printf("Replacing image prototype %s... ", prototype);
    rc = smPrototype_Replace_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            prototype, recordCount, (vmApiPrototypeRecordList *) record, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Prototype_Replace_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Prototype_Replace_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}
