/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Add a disk to a disk pool defined in the EXTENT CONTROL.
 * The disk has to already be attached to SYSTEM.
 *
 * @param $1: Function type, where
 *         (4) Define region as full volume, add to group
 *         (5) Add existing region to group
 * @param $2: Region name
 * @param $3: Volume label
 * @param $4: Group name
 *
 * @return 0 If the disk was added to the disk pool
 *         1 If given invalid parameters
 *         2 If the disk failed to be added to the disk pool
 */
int main(int argC, char * argV[]) {
    int rc;
    int functionType = 0;
    char* userid = "ZHCP";
    char* regionName = NULL;
    char* volLabel = "";
    char* groupName = "";
    vmApiImageVolumeSpaceDefineDmOutput* output;
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
        printf("Add a disk to a disk pool defined in the EXTENT CONTROL.\n"
            "The disk has to already be attached to SYSTEM.\n\n"
            "Usage: adddisk2pool [@params]\n"
            "@param $1: Function type, where the value can be either\n"
            "  (4) Define region as full volume and add to group\n"
            "  (5) Add existing region to group\n"
            "@param $2: Region name\n"
            "@param $3: Volume label\n"
            "@param $4: Group name\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC > 5) {
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

    if (functionType == 4) {
        volLabel = argV[3];
        groupName = argV[4];
        printf("Adding %s to %s... ", regionName, groupName);
    } else if (functionType == 5) {
        groupName = argV[3];
        printf("Adding %s to %s... ", regionName, groupName);
    } else {
        printf("Error: Function type must be 4 or 5\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    rc = smImage_Volume_Space_Define_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            userid, functionType, regionName, volLabel, -1, -1,  // Starting cylinder and number of cylinders
            groupName, 0,  // Group name and device type
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
