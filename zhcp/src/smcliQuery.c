/**
 * IBM (C) Copyright 2013,2016 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliQuery.h"
#include "wrapperutils.h"

int queryABENDDump(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int entryCount = 0;
    int option;
    char abend_dump_loc[75];
    char * targetIdentifier = NULL;
    char * entryArray[1];
    vmApiQueryAbendDumpOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:k:h?")) != -1)
        switch (option) {
            case 'T':
                targetIdentifier = optarg;
                break;

            case 'k':
                if (!optarg) {
                    return INVALID_DATA;
                }
                if (entryCount < 1) {
                    entryArray[entryCount] = optarg;
                    entryCount++;
                } else {
                    printf(" Error Too many -k values entered.\n");
                    return INVALID_DATA;
                }
                break;

            case 'h':
                printf("NAME\n"
                    "  Query_ABEND_Dump\n\n"
                    "SYNOPSIS\n"
                    "  smcli Query_ABEND_Dump [-T] targetIdentifier\n"
                    "    [-k] entry1 [-k] entry2 ...\n\n"
                    "DESCRIPTION\n"
                    "  Use Query_ABEND_Dump to display the current ABEND dumps that appear in the\n"
                    "  OPERATNS userid's reader or have already been processed to the dump processing\n"
                    "  location specified in the DMSSICNF COPY file.\n\n"
                    "  The following options are required:\n"
                    "    -T  This must match an entry in the authorization file\n"
                    "    -k  A keyword=value item location set to the where to query.\n"
                    "        It must be inside single quotes. For example -k 'key=value'\n. "
                    "        Possible keywords are:\n"
                    "          Location\n"
                    "          RDR: Query ABEND dumps in the reader (unprocessed).\n"
                    "          SFS: Query ABEND dumps in the VMSYSU:OPERATNS. SFS directory (processed).\n"
                    "          ALL: Query ABEND dumps both in the reader and the SFS directory.\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!targetIdentifier ||  entryCount < 1)  {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    rc = smQuery_ABEND_Dump(vmapiContextP, "", 0, "",  targetIdentifier, entryCount, entryArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Query_ABEND_Dump", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Query_ABEND_Dump", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        for (i = 0; i < output->dumpStrCount; i++) {
            if (output->abendDumpStructure[i].abend_dump_loc == 1) {
                strcpy(abend_dump_loc, "Reader (unprocessed)");
            } else if (output->abendDumpStructure[i].abend_dump_loc == 2) {
                strcpy(abend_dump_loc, "SFS directory (processed)");
            } else {
                strcpy(abend_dump_loc, "Error Invalid data received!! ");
                return 1;
            }
            printf("abend_dump_loc: %s\n",  abend_dump_loc);
            printf("abend_dump_id: %s\n",   output->abendDumpStructure[i].abend_dump_id);
            printf("abend_dump_date: %s\n", output->abendDumpStructure[i].abend_dump_date);
            printf("abend_dump_dist: %s\n", output->abendDumpStructure[i].abend_dump_dist);
        }
    }
    return rc;
}

int queryAllDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int j;
    int recCount;
    int recLen;
    int entryCount = 0;
    int option;
    char format[10+1] = "";
    char line[80];
    char * targetIdentifier = NULL;
    char * entryArray[1];
    int bytesUsed;
    int entryLength;
    char * stringStart;
    vmApiQueryAllDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:k:h?")) != -1)
        switch (option) {
            case 'T':
                targetIdentifier = optarg;
                break;

            case 'k':
                if (!optarg) {
                    return INVALID_DATA;
                }
                if (entryCount < 1) {
                    entryArray[entryCount] = optarg;
                    entryCount++;
                } else {
                    printf("ERROR: Too many -k values entered\n");
                    return INVALID_DATA;
                }
                break;

            case 'h':
                printf("NAME\n"
                    "  Query_All_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Query_All_DM [-T] targetIdentifier  [-k] entry\n\n"
                    "DESCRIPTION\n"
                    "  Use Query_All_DM to obtain the contents of the entire system directory.\n\n"
                    "  The following options are required:\n"
                    "    -T  This must match an entry in the authorization file\n"
                    "    -k  A keyword=value item location set to the where to query.\n"
                    "        It must be inside single quotes. For example -k 'key=value'.\n"
                    "         Possible settings are:\n"
                    "            FORMAT=YES  Output data formatted.\n"
                    "            FORMAT=NO   Output data unformatted..\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!targetIdentifier ||  entryCount < 1)  {
        printf("\nERROR: Missing required options\n");
        return 1;
    }
    // Fill the var format with the input for FORMAT
    // Convert the input to upper case for the strcmp test
    memset(format, 0, 11);
    for (i=0; i < strlen(entryArray[0]); i++) {
        format[i] = toupper(entryArray[0][i]);
    }

    if (strcmp(format, "FORMAT=YES") == 0) {
        // Format = YES
        rc = smQuery_All_DM_YES(vmapiContextP, "", 0, "",  targetIdentifier, entryCount, entryArray, &output);

        if (rc) {
            printAndLogSmapiCallReturnCode("Query_All_DM", rc, vmapiContextP);
        } else if (output->common.returnCode || output->common.reasonCode) {
            // Handle SMAPI return code and reason code
            rc = printAndLogSmapiReturnCodeReasonCodeDescription("Query_All_DM", output->common.returnCode,
                    output->common.reasonCode, vmapiContextP);
        } else {
            for (i = 0; i < output->directoryEntriesArrayCount; i++) {
                printf("Directory Entry_type: %d\n", output->directoryEntryArray[i].directoryEntryType);
                printf("Directory Entry ID: %s\n", output->directoryEntryArray[i].directoryEntryId);
                // Print out directory entry strings
                bytesUsed = 0;
                stringStart = output->directoryEntryArray[i].directoryEntryData;
                entryLength = output->directoryEntryArray[i].directoryEntryDataLength;
                while (bytesUsed < entryLength) {
                    j = strlen(stringStart)+1;
                    printf("%s\n", stringStart);
                    stringStart += j;
                    bytesUsed += j;
                }
                printf("\n");
            }
        }
    } else {
        // Format = NO
        rc = smQuery_All_DM_NO(vmapiContextP, "", 0, "",  targetIdentifier, entryCount, entryArray, &output);

        if (rc) {
            printAndLogSmapiCallReturnCode("Query_All_DM", rc, vmapiContextP);
        } else if (output->common.returnCode || output->common.reasonCode) {
            // Handle SMAPI return code and reason code
            rc = printAndLogSmapiReturnCodeReasonCodeDescription("Query_All_DM", output->common.returnCode,
                    output->common.reasonCode, vmapiContextP);
        } else {
            for (i = 0; i < output->directoryEntriesArrayCount; i++) {
                printf("Directory Entry_type: %d\n", output->directoryEntryArray[i].directoryEntryType);
                printf("Directory Entry ID: %s\n", output->directoryEntryArray[i].directoryEntryId);
                // Print out directory entry
                recCount = output->directoryEntryArray[i].directoryEntryArrayCount;
                for (j = 0; j < recCount; j++ ) {
                    recLen = output->directoryEntryArray[i].directoryEntryRecordArray[j].directoryEntryRecordLength - 8;
                    memset(line, 0x20, 80);
                    strncpy(line, output->directoryEntryArray[i].directoryEntryRecordArray[j].directoryEntryRecord, recLen);
                    trim(line);
                    printf("%s\n", line);
                }
                printf("\n");
            }
        }
    }
    return rc;
}

int queryAPIFunctionalLevel(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    vmApiQueryApiFunctionalLevelOutput * output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Query_API_Functional_Level\n\n"
                    "SYNOPSIS\n"
                    "  smcli Query_API_Functional_Level [-T] image_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Query_API_Functional_Level to obtain the support level of the server\n"
                    "  and functions, as follows:\n"
                    "    For z/VM V5.3, this API will provide a return and reason code of 0/0\n"
                    "    For z/VM V5.4, this API will provide a return and reason code of 0/540\n"
                    "    For z/VM V6.1, this API will provide a return and reason code of 0/610\n"
                    "    For the updated z/VM V6.1 SPE release, this API will provide a return\n"
                    "    and reason code of 0/611.\n"
                    "    For z/VM V6.2, this API will provide a return and reason code of 0/620.\n"
                    "    For z/VM V6.3, this API will provide a return and reason code of 0/630.\n"
                    "    For z/VM V6.4, this API will provide a return and reason code of 0/640.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smQuery_API_Functional_Level(vmapiContextP, "", 0, "", image, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Query_API_Functional_Level", rc, vmapiContextP);
    } else if ((output->common.returnCode == 0) && (output->common.reasonCode == 0)) {
        printf("The API functional level is z/VM V5.3\n");
    } else if ((output->common.returnCode == 0) && (output->common.reasonCode == 540)) {
        printf("The API functional level is z/VM V5.4\n");
    } else if ((output->common.returnCode == 0) && (output->common.reasonCode == 610)) {
        printf("The API functional level is z/VM V6.1\n");
    } else if ((output->common.returnCode == 0) && (output->common.reasonCode == 611 || output->common.reasonCode == 612)) {
        printf("The API functional level is the updated z/VM V6.1 SPE release\n");
    } else if ((output->common.returnCode == 0) && (output->common.reasonCode == 620 || output->common.reasonCode == 621)) {
        printf("The API functional level is z/VM V6.2\n");
    } else if ((output->common.returnCode == 0) && (output->common.reasonCode == 630 || output->common.reasonCode == 631 || output->common.reasonCode == 632)) {
        printf("The API functional level is z/VM V6.3\n");
    } else if ((output->common.returnCode == 0) && ((output->common.reasonCode >= 640 || output->common.reasonCode < 650))) {
        printf("The API functional level is z/VM V6.4\n");
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Query_API_Functional_Level", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int queryAsynchronousOperationDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int operationId = -1;
    char * image = NULL;
    vmApiQueryAsynchronousOperationDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:i:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'i':
                operationId = atoi(optarg);
                break;

            case 'h':
                printf("NAME\n"
                    "  Query_Asynchronous_Operation_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Query_Asynchronous_Operation_DM [-T] image_name [-i] operation_id\n\n"
                    "DESCRIPTION\n"
                    "  Use Query_Asynchronous_Operation_DM to query the status of an asynchronous\n"
                    "  directory manager operation.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n"
                    "    -i    The identifier of the operation to be queried\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image || (operationId < 0)) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smQuery_Asychronous_Operation_DM(vmapiContextP, "", 0, "", image, operationId, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("System_SCSI_Disk_Query", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Query_Asychronous_Operation_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int queryDirectoryManagerLevelDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    vmApiQueryDirectoryManagerLevelDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Query_Directory_Manager_Level_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Query_Directory_Manager_Level_DM [-T] image_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Query_Directory_Manager_Level_DM to query the directory manager\n"
                    "  that is being used and its functional level.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smQuery_Directory_Manager_Level_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            image, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Query_Directory_Manager_Level_DM", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Query_Directory_Manager_Level_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        printf("%s\n", output->directoryManagerLevel);
    }
    return rc;
}
