/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliProcess.h"
#include "wrapperutils.h"

int processABENDDump(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int entryCount = 0;
    int option;
    char * targetIdentifier = NULL;
    char * entryArray[1];
    vmApiProcessAbendDumpOutput * output;

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
                    "  Process_ABEND_Dump\n\n"
                    "SYNOPSIS\n"
                    "  smcli Process_ABEND_Dump [-T] targetIdentifier\n"
                    "    [-k] 'entry1' [-k] 'entry2' ...\n\n"
                    "DESCRIPTION\n"
                    "  Use Process_ABEND_Dump to instruct the dump processing userid to process one\n"
                    "  or more ABEND dumps from its reader and place them in the dump processing\n"
                    "  location specified in the DMSSICNF COPY file.\n"
                    "  system.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file.\n"
                    "    -k    A keyword=value item.\n"
                    "          They may be specified in any order. They must be inside\n"
                    "          single quotes. For example -k 'key=value'. Possible keywords are:\n"
                    "              spoolid - The spool ID of the ABEND dump to be processed, or ALL\n"
                    "                        to process all remaining ABEND dumps. If not specified,\n"
                    "                        the next ABEND dump is processed.\n");
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

    printf("Processing one or more ABEND dumps from the %s Reader ... ", targetIdentifier);
    rc = smProcess_ABEND_Dump(vmapiContextP, "", 0, "",  targetIdentifier, entryCount, entryArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Process_ABEND_Dump", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Process_ABEND_Dump", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}
