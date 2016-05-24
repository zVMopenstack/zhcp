/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Removes a disk from a virtual server's directory entry.
 *
 * @param $1: The name of the guest from which a disk is to be removed
 * @param $2: The virtual device address of the volume which is to be removed
 *
 * @return 0 If the disk is not in the image's directory entry upon completion
 *         1 If given invalid parameters
 *         2 If the disk allocation failed
 */
int main(int argC, char* argV[]) {
    int rc;
    char* image = NULL;
    char* vdev = NULL;
    vmApiImageDiskDeleteDmOutput* output;
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
        printf("Removes a disk from a virtual server's directory entry.\n\n"
            "Usage: removemdisk [@params]\n"
            "@param $1: The name of the guest from which a disk is to be removed\n"
            "@param $2: The virtual device address of the volume which is to be removed\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC != 3) {
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

    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image) || isDevNumberInvalid(vdev)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    printf("Removing disk %s on %s... ", vdev, image);
    rc = smImage_Disk_Delete_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            image, vdev, 0,  // Use default setting for whether to erase data.
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
