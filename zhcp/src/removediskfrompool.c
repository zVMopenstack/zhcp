/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Remove a disk from a disk pool defined in the EXTENT CONTROL.
 *
 * @param $1: Function type, where
 *         (1) Remove region
 *         (2) Remove region from group
 *         (3) Remove region from all groups
 *         (7) Remove entire group
 * @param $2: Volume label
 * @param $3: Group name
 *
 * @return 0 If the disk was removed to the disk pool
 *         1 If given invalid parameters
 *         2 If the disk failed to be removed from the disk pool
 */
int main(int argC, char * argV[]) {
    int rc;
    int functionType = 0;
    char* userid = "ZHCP";
    char* regionName = NULL;
    char* volLabel = "";
    char* groupName = "";
    vmApiImageVolumeSpaceRemoveDmOutput* output;
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
        printf("Remove a disk from a disk pool defined in the EXTENT CONTROL.\n\n"
            "Usage: removediskfrompool [@params]\n"
            "@param $1: Function type, where the value can be either\n"
            "  (1) Remove region\n"
            "  (2) Remove region from group\n"
            "  (3) Remove region from all groups\n"
            "  (7) Remove entire group\n"
            "@param $2: Region name\n"
            "@param $3: Group name\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC > 4) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }
    // Get inputs
    functionType = atoi(argV[1]);
    regionName = argV[2];

    if (functionType == 1 || functionType == 3) {
        printf("Removing %s... ", regionName);
    } else if (functionType == 2) {
        groupName = argV[3];
        printf("Removing %s from %s... ", regionName, groupName);
    } else if (functionType == 7) {
        groupName = argV[3];
        printf("Removing %s... ", groupName);
    } else {
        printf("Error: Function type must be 1, 2, 3, or 7\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    rc = smImage_Volume_Space_Remove_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            userid, functionType, regionName, volLabel, groupName,  // Region name, volume ID, and group name
            &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 0)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
            "  Reason Code: %d\n", output->common.returnCode, output->common.reasonCode);
    } else {
        printf("Done\n");
    }

    TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
    /* Clean up for memory context */
    FREE_CONTEXT_MEMORY(&vmapiContext);
    return rc;
}
