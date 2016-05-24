/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Prints out the specified directory entry.
 *
 * @param $1: The name of the virtual server which is to be listed
 *
 * @return 0 If the image record was successfully printed
 *         1 If given invalid parameters
 *         2 If the image could not be retrieved
 */
int main(int argC, char * argV[]) {
    int rc = 0;  // Return code
    char * image = NULL;
    vmApiImageQueryDmOutput * output;
    vmApiInternalContext vmapiContext;
    smMemoryGroupContext memContext;
    extern struct _smTrace externSmapiTraceFlags;

    /* Initialize context and memory structure */
    INIT_CONTEXT_AND_MEMORY(&vmapiContext, &memContext, &externSmapiTraceFlags, rc);
    if (rc)  {
        // If initialize of context and memory failed, probably out of memory
        return rc;
    }
    readTraceFile(&vmapiContext);
    TRACE_ENTRY_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL);

    if (argC < 2 || !strcmp(argV[1], "-h") || !strcmp(argV[1], "--help")) {
        printf("Prints out the specified directory entry.\n\n"
            "Usage: getuserentry [@params]\n"
            "@param $1: The name of the virtual server which is to be listed\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC != 2) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get inputs
    image = argV[1];

    // Query DirMaint directory entry
    rc = smImage_Query_DM(&vmapiContext, "", 0, "", image, &output, false);

    // Handle return code and reason code
    if (rc || (output->common.returnCode && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 0)) {
        printf("Failed\n");

        rc ? printf("  Return code: %d\n", rc) : printf("  Return code: %d\n"
                    "  Reason code: %d\n", output->common.returnCode, output->common.reasonCode);
    } else {
        // Print out DirMaint directory entry
        int recCount = output->imageRecordCount;
        int recLen = output->imageRecordList[0].imageRecordLength - 8;

        // Line to print
        char line[recLen], chs[4];

        if (recCount > 0) {
            int i;
            int token = 0;

            // Loop through image record and print out directory entry
            for (i = 0; i < recCount; i++) {
                memset(line, 0, recLen);
                memcpy(line, output->imageRecordList[i].imageRecord, recLen);

                // Print lines that are not comment
                trim(line);

                // Copy first 4 characters
                strncpy(chs, line, 4);

                // If first 4 characters does not have * (comment)
                if (!strstr(chs, "*")) {
                    // Print directory entry line
                    printf("%s\n", line);
                }
            }
        }
    }

    TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
    /* Clean up for memory context */
    FREE_CONTEXT_MEMORY(&vmapiContext);
    return rc;
}
