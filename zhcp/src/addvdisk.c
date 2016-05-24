/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Adds a v-disk to a virtual server's directory entry.
 *
 * @param $1: The name of the guest to which a disk is to be added
 * @param $2: The virtual device address for the newly-allocated volume
 * @param $3: The size of the new disk, in 512-byte blocks
 *
 * @return 0 If the disk was added successfully
 *         1 If given invalid parameters
 *         2 If disk allocation failed
 */
int main(int argC, char* argV[]) {
    int rc;
    int size =  0;
    char* image = NULL;
    char* vdev =  NULL;
    char* mode =  NULL;
    vmApiImageDiskCreateDmOutput* output;
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
        printf("Adds a v-disk to a virtual server's directory entry.\n\n"
            "Usage: addvdisk [@params]\n"
            "@param $1: The name of the guest to which a disk is to be added\n"
            "@param $2: The virtual device address for the newly-allocated volume\n"
            "@param $3: The size of the new disk, in 512-byte blocks\n"
            "@param $4: The mode under which the disk is to be linked by the user\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC != 5) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get inputs
    image = argV[1];
    vdev = argV[2];
    size = atoi(argV[3]);
    mode = argV[4];

    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    printf("Adding V-Disk %s to %s... ", vdev, image);
    rc = smImage_Disk_Create_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password
            image, vdev, "FB-512",  // Required for V-Disk devices
            "V-DISK", "NONE",  // No disk pool (value is ignored)
            2,  // Allocation unit is BLK0512
            size, mode,  // Disk size and mode
            0, "",  // Formatting unspecified, no label
            "", "", "",  // Read, write, and multi passwords
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
