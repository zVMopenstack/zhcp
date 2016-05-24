/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Get a list of defined disk pools.
 *
 * @param $1: The name of the guest to get a list of disk pools for
 *
 * @return 0 If the disk pools were successfully printed
 *         1 If given invalid parameters
 *         2 If the disk pools could not be retrieved
 */
int main(int argC, char * argV[]) {
    int rc;
    int i;
    char* image = NULL;
    char * poolName = NULL;
    vmApiImageRecord * record;
    vmApiImageVolumeSpaceQueryDmOutput * output;
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
        printf("Get a list of defined disk pools.\n\n"
            "Usage: getdiskpoolnames [@params]\n"
            "@param $1: The name of the guest to get a list of disk pools for\n");
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

    // Get input
    image = argV[1];
    rc = smImage_Volume_Space_Query_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            image, 1,  // Query definition
            3,  // Query group
            "*",  // All areas
            &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 0)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
                    "  Reason Code: %d\n", output->common.returnCode, output->common.reasonCode);
    } else {
        record = output->recordList;
        for (i = 0; i < output->recordCount; i++) {
            // Get disk pool name from each record
            const char * delim = " ";
            poolName = strtok_r(record->imageRecord, delim, (char **) &vmapiContext);
            if (poolName) {
                printf("%s\n", poolName);
            }
            record++;
        }
    }

    TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
    /* Clean up for memory context */
    FREE_CONTEXT_MEMORY(&vmapiContext);
    return rc;
}
