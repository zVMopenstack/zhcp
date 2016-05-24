/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "vmapiVirtual.h"
#include "wrapperutils.h"

/**
 * Connect network adapter to a GuestLAN.
 *
 * @param $1: The guest for which a LAN connection is being created
 * @param $2: The virtual device address for the new adapter
 * @param $3: The name of the guest LAN to connect the virtual image
 * @param $4: The guest owning the guest LAN to be connected
 *
 * @return 0 If the NIC was added successfully
 *         1 If given invalid parameters
 *         2 If the NIC could not be connected to the VSwitch
 */
int main(int argC, char * argV[]) {
    int rc;
    char* image = NULL;
    char* address = NULL;
    char* lan = NULL;
    char* owner = NULL;
    vmApiVirtualNetworkAdapterConnectLanDmOutput* output;
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
        printf("Connect network adapter to a GuestLAN.\n\n"
            "Usage: connectnic2guestlan [@params]\n"
            "@param $1: The guest for which a LAN connection is being created\n"
            "@param $2: The virtual device address for the new adapter\n"
            "@param $3: The name of the guest LAN to connect the virtual image\n"
            "@param $4: The guest owning the guest LAN to be connected\n");
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
    address = argV[2];
    lan = argV[3];
    owner = argV[4];
    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image) || isImageNameInvalid(owner)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }
    printf("Connecting NIC %s to GuestLan %s on %s... ", address, lan, image);

    rc = smVirtual_Network_Adapter_Connect_LAN_DM(&vmapiContext, "", 0, "",
            image, address, lan, owner, &output);

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
