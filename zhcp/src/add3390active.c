/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Adds a 3390 (ECKD) disk that is defined in a virtual image's directory
 * entry to that virtual server's active configuration
 *
 * @param $1: The name of the guest to which a disk is to be added
 * @param $2: The virtual device address for the newly-allocated volume
 * @param $3: The mode under which the disk is to be linked by the user
 *
 * @return 0 If the disk was added successfully
 *         1 If given invalid parameters
 *         2 If disk allocation failed
 */
int main(int argC, char* argV[]) {
    int rc;
    char* image = NULL;
    char* vdev = NULL;
    char* mode = NULL;
    vmApiImageDiskCreateOutput * output;
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
        printf("Adds a 3390 (ECKD) disk that is defined in a virtual image's directory\n"
            "entry to that virtual server's active configuration.\n\n"
            "Usage: add3390active [@params]\n"
            "@param $1: The name of the guest to which a disk is to be added\n"
            "@param $2: The virtual device address for the newly-allocated volume\n"
            "@param $3: The mode under which the disk is to be linked by the user\n");
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
    vdev = argV[2];
    mode = argV[3];
    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }
    printf("Adding disk %s to %s(active)... ", vdev, image);
    rc = smImage_Disk_Create(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
             image, vdev, mode, &output);

    if (rc || output->common.returnCode || output->common.reasonCode) {
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
