/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Removes a dedicated device from a virtual server's directory entry.
 *
 * @param $1: The name of the guest from which a device is to be removed
 * @param $2: The virtual device address assigned to the device
 *
 * @return 0 If the dedicated device was removed successfully
 *         1 If given invalid parameters
 *         2 If device undedication failed.
 */
int main(int argC, char* argV[]) {
    int rc;
    char* image = argV[1];
    char* vdev = argV[2];
    vmApiImageDeviceUndedicateDmOutput* output;
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
        printf("Removes a dedicated device from a virtual server's directory entry.\n\n"
            "Usage: undedicatedevice [@params]\n"
            "@param $1: The name of the guest from which a device is to be removed\n"
            "@param $2: The virtual device address assigned to the device\n");
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

    if (isImageNameInvalid(image)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    printf("Removing device %s on %s... ", vdev, image);
    rc = smImage_Device_Undedicate_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
             image, vdev, &output);

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
