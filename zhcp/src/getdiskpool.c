/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Get the free and used space in a specified disk pool.
 *
 * @param $1: The name of the guest to get the disk pool from
 * @param $2: The name of the disk pool
 * @param $3: Free or used space in the disk pool
 *
 * @return 0 If the disk pool was successfully printed
 *         1 If given invalid parameters
 *         2 If the disk pool could not be retrieved
 */
int main(int argC, char * argV[]) {
    int rc;
    int i;
    int space;
    char temp[1024];
    char * token;
    char diskStr[256];
    char * image = NULL;
    char * pool = NULL;
    char * spaceStr = NULL;
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
        printf("Get the free and used space in a specified disk pool.\n\n"
            "Usage: getdiskpool [@params]\n"
            "@param $1: The name of the guest to get the disk pool from\n"
            "@param $2: The name of the disk pool\n"
            "@param $3: Free or used space in the disk pool\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC != 4) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get inputs
    image = argV[1];
    pool = argV[2];
    spaceStr = argV[3];

    // Get the disk space
    if (strcmp(spaceStr, "free") == 0) {
        space = 2;
    } else if (strcmp(spaceStr, "used") == 0) {
        space = 3;
    } else {
        printf("Error: Disk pool space must be free or used\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get disk pool used space
    rc = smImage_Volume_Space_Query_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            image, space,  // Used space
            3,  // Query group
            pool, &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 0)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
                    "  Reason Code: %d\n", output->common.returnCode, output->common.reasonCode);
    } else {
        printf("#VolID DevType StartAddr Size\n");

        record = output->recordList;
        for (i = 0; i < output->recordCount; i++) {
            strncpy(temp, record->imageRecord, record->imageRecordLength);

            const char * delim = " ";
            token = strtok_r(temp, delim, (char **) &vmapiContext);  // VolId
            strcpy(diskStr, token);
            strcat(diskStr, delim);
            token = strtok_r(NULL, delim, (char **) &vmapiContext);  // Device type
            strcat(diskStr, token);
            strcat(diskStr, delim);
            token = strtok_r(NULL, delim, (char **) &vmapiContext);  // Start address
            strcat(diskStr, token);
            strcat(diskStr, delim);
            token = strtok_r(NULL, delim, (char **) &vmapiContext);  // Size
            strcat(diskStr, token);

            printf("%s\n", diskStr);
            record++;
        }
    }

    TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
    /* Clean up for memory context */
    FREE_CONTEXT_MEMORY(&vmapiContext);
    return rc;
}
