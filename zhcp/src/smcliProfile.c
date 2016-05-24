/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliProfile.h"
#include "wrapperutils.h"

int profileCreateDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int recordCount = 0;
    int c;
    char * profile = NULL;
    char * profileFile = NULL;
    FILE * fp = NULL;
    vmApiProfileCreateDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "-T:f:h?")) != -1)
        switch (option) {
            case 'T':
                profile = optarg;
                break;

            case 'f':
                profileFile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Profile_Create_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Profile_Create_DM [-T] profile_name [-f] profile_file\n\n"
                    "DESCRIPTION\n"
                    "  Use Profile_Create_DM to create a profile directory entry to be included\n"
                    "  in the definition of a virtual image in the directory.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the profile to be created\n"
                    "    -f    The profile directory entry file\n");
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

    if (!profile || !profileFile) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    // Open the user entry file
    fp = fopen(profileFile, "r");
    if (fp != NULL) {
        // Count the number of lines and set the record count to it
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {
                recordCount++;
            }
        }
    } else {
        printf("\nERROR: Failed to open file %s\n", profileFile);
        return 1;
    }
    // Reset position to start of file
    rewind(fp);

    // Create image record
    vmApiProfileRecord record[recordCount];
    int i = 0, LINE_SIZE = 72;
    char line[recordCount][LINE_SIZE];
    char * ptr;
    while (fgets(line[i], LINE_SIZE, fp) != NULL) {
        // Replace newline with null terminator
        ptr = strstr(line[i], "\n");
        if (ptr != NULL)
            strncpy(ptr, "\0", 1);

        record[i].profileRecordLength = strlen(line[i]);
        record[i].recordData = line[i];
        i++;
    }

    // Close file
    fclose(fp);

    printf("Creating profile directory entry %s... ", profile);
    rc = smProfile_Create_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            profile, recordCount, (vmApiProfileRecord *) record,  // Image record
            &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Profile_Create_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Profile_Create_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int profileDeleteDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * profile = NULL;
    vmApiProfileDeleteDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "-T:h?")) != -1)
        switch (option) {
            case 'T':
                profile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Profile_Delete_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Profile_Delete_DM [-T] profile_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Profile_Delete_DM to delete a profile directory entry.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the profile to be deleted\n");
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

    printf("Deleting profile directory entry %s... ", profile);
    rc = smProfile_Delete_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            profile, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Profile_Delete_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Profile_Delete_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int profileLockDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * profile = NULL;
    vmApiProfileLockDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "-T:h?")) != -1)
        switch (option) {
            case 'T':
                profile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Profile_Lock_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Profile_Lock_DM [-T] profile_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Profile_Lock_DM to lock a profile directory entry.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the profile to be locked.\n");
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

    printf("Locking profile directory entry %s... ", profile);
    rc = smProfile_Lock_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            profile, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Profile_Lock_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Profile_Lock_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int profileLockQueryDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int i;
    int devAddressDevLockedBySize;
    char * profile = NULL;
    char *token;
    char * buffer;  // Character pointer whose value is preserved between successive related calls to strtok_r
    char * blank = " ";
    char lockedType[7 + 1];
    char profileLockedBy[8 + 1];
    char devAddress[4 + 1];
    char devLockedBy[8 + 1];
    char tempBuff[14];
    vmApiProfileLockQueryDmOutput * output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "-T:h?")) != -1)
        switch (option) {
            case 'T':
                profile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Profile_Lock_Query_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Profile_Lock_Query_DM [-T] profile_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Profile_Lock_Query_DM to query the status of whether a directory manager\n"
                    "  lock is in effect for a specific profile.\n\n"
                    "  The following options are required:\n"
                    "    -T   The name of the profile for which the directory lock status is being\n"
                    "         queried.\n");
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

    rc = smProfile_Lock_Query_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            profile,  // Profile name
            &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Profile_Lock_Query_DM", rc, vmapiContextP);
    } else if ((output->common.returnCode == 0) && (output->common.reasonCode == 12 )) {
        // Obtain lockedType
        token = strtok_r(output->lockedTypeProfileLockedBy, blank, &buffer);
        if (token != NULL) {
            strcpy(lockedType, token);
        } else {
            printf("ERROR: lockedType is NULL\n");
            return 1;
        }
        if (strcmp(lockedType, "PROFILE") == 0) {
            // Obtain profileLockedBy
            token = strtok_r(NULL, "\0", &buffer);
            if (token != NULL) {
                strcpy(profileLockedBy, token);
            } else {
                printf("ERROR: profileLockedBy is NULL\n");
                return 1;
            }
            printf("Locked type: %s\n"
                   "Image locked by: %s\n",
                   lockedType, profileLockedBy);
        } else {
            printf("Locked type: %s\n", lockedType);
        }

        for (i = 0; i < output->profileDevLockInfoRecordCount; i++) {
            memset(tempBuff, 0x00, 14);
            devAddressDevLockedBySize = strlen((char *)output->lockDevList[i].devAddressDevLockedBy);
            strncpy(tempBuff, (char *)output->lockDevList[i].devAddressDevLockedBy, devAddressDevLockedBySize);
            trim(tempBuff);
            tempBuff[devAddressDevLockedBySize + 1] = '\0';
            // Get devAddress
            token = strtok_r(tempBuff, blank, &buffer);
            if (token != NULL) {
                strcpy(devAddress, token);
            } else {
                printf("Error devAddress is NULL!!\n");
                rc = 1;
                break;
            }

            // Obtain devLockedBy
            token = strtok_r(NULL, "\0", &buffer);
            if (token != NULL) {
                strcpy(devLockedBy, token);
            } else {
                printf("ERROR: devLockedBy is NULL\n");
                rc =1;
                break;
            }

            printf("Device address: %s\n"
                   "Device locked by: %s\n\n",
                   devAddress, devLockedBy);
        }
    } else if ((output->common.returnCode == 0) && (output->common.reasonCode == 24)) {
        printf("%s is Unlocked...\n", profile);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Profile_Lock_Query_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int profileQueryDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int recCount;
    int i;
    char line[72+1];
    char * profile = NULL;
    vmApiProfileQueryDmOutput * output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "-T:h?")) != -1)
        switch (option) {
            case 'T':
                profile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Profile_Query_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Profile_Query_DM [-T] profile_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Profile_Query_DM to query a profile directory entry.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the profile being queried\n");
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

    rc = smProfile_Query_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            profile,  // Profile name
            &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Profile_Query_DM", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Profile_Query_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        // Print out directory profile
        recCount = output->profileRecordCount;
        if (recCount > 0) {
        	printf("\n");
            // Loop through profile records and print out profile entry
            for (i = 0; i < output->profileRecordCount; i++) {
                memset(line, 0, 73);
                memcpy(line, output->profileRecordList[i].recordData, 72);
                trim(line);
                printf("%s\n", line);
            }
        }
    }
    return rc;
}

int profileReplaceDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int recordCount = 0;
    int c;
    char * profile = NULL;
    char * profileFile = NULL;
    FILE * fp = NULL;
    vmApiProfileReplaceDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "-T:f:h?")) != -1)
        switch (option) {
            case 'T':
                profile = optarg;
                break;

            case 'f':
                profileFile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Profile_Replace_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Profile_Replace_DM [-T] profile_name [-f] profile_file\n\n"
                    "DESCRIPTION\n"
                    "  Use Profile_Replace_DM to replace the definition of a profile to be\n"
                    "  included in a virtual image in the directory.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the profile directory entry to be replaced\n"
                    "    -f    The profile directory entry file");
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

    if (!profile || !profileFile) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    // Open the user entry file
    fp = fopen(profileFile, "r");
    if (fp != NULL) {
        // Count the number of lines and set the record count to it
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {
                recordCount++;
            }
        }
    } else {
        printf("\nERROR: Failed to open file %s\n", profileFile);
        return 1;
    }
    // Reset position to start of file
    rewind(fp);

    // Create image record
    vmApiProfileRecord record[recordCount];
    int i = 0, LINE_SIZE = 72;
    char line[recordCount][LINE_SIZE];
    char * ptr;
    while (fgets(line[i], LINE_SIZE, fp) != NULL) {
        // Replace newline with null terminator
        ptr = strstr(line[i], "\n");
        if (ptr != NULL)
            strncpy(ptr, "\0", 1);

        record[i].profileRecordLength = strlen(line[i]);
        record[i].recordData = line[i];
        i++;
    }

    // Close file
    fclose(fp);

    printf("Replacing definition of %s... ", profile);
    rc = smProfile_Replace_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            profile, recordCount, (vmApiProfileRecord *) record,  // Image record
            &output);


    if (rc) {
        printAndLogSmapiCallReturnCode("Profile_Replace_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Profile_Replace_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int profileUnlockDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * profile = NULL;
    vmApiProfileUnlockDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "-T:h?")) != -1)
        switch (option) {
            case 'T':
                profile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Profile_Unlock_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Profile_Unlock_DM [-T] profile_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Profile_Unlock_DM to unlock a profile directory entry.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the profile to be unlocked.\n");
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

    printf("UnLocking profile directory entry %s... ", profile);
    rc = smProfile_Unlock_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            profile, &output);


    if (rc) {
        printAndLogSmapiCallReturnCode("Profile_Unlock_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Profile_Unlock_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}
