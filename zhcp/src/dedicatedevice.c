/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Adds a dedicated device to a virtual image's directory entry.
 *
 * @param $1: The name of the guest to which a device is to be added
 * @param $2: The virtual device address to be assigned to the device
 * @param $3: The device's real address
 *
 * @return 0 If the dedicated device was added successfully
 *         1 If given invalid parameters
 *         2 If device dedication failed
 */
int main(int argC, char* argV[]) {
    int rc;
    int readonly = 0;
    char* image = NULL;
    char* vdev = NULL;
    char* rdev = NULL;
    vmApiImageDeviceDedicateDmOutput* output;
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
        printf("Adds a dedicated device to a virtual image's directory entry.\n\n"
            "Usage: dedicatedevice [@params]\n"
            "@param $1: The name of the guest to which a device is to be added\n"
            "@param $2: The virtual device address to be assigned to the device\n"
            "@param $3: The device's real address\n"
            "@param $4: Specify 1 if the virtual device is to be in read-only mode,\n"
            "  Otherwise, specify a 0\n");
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
    rdev = argV[3];
    readonly = atoi(argV[4]);
    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    printf("Dedicating device %s as %s to %s... ", rdev, vdev, image);
    rc = smImage_Device_Dedicate_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            image, vdev, rdev, readonly, &output);

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
