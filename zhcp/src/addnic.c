/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include <string.h>
#include "vmapiVirtual.h"
#include "wrapperutils.h"

/**
 * Adds a network adapter to a virtual server's directory entry.
 *
 * @param $1: The guest for which a NIC is being allocated
 * @param $2: The virtual device address of the NIC being allocated
 * @param $3: The type of NIC which is to be allocated
 * @param $4: The number of VDEV channels associated with this NIC
 *
 * @return 0 If the NIC was added successfully
 *         1 If given invalid parameters
 *         2 If the NIC definition failed
 */
int main(int argC, char * argV[]) {
    int rc;
    int typecode;
    int devcount = 0;
    char* image = NULL;
    char* address = NULL;
    char* type = NULL;
    vmApiVirtualNetworkAdapterCreateDmOutput* output;
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
        printf("Adds a network adapter to a virtual server's directory entry.\n\n"
            "Usage: addnic [@params]\n"
            "@param $1: The guest for which a NIC is being allocated\n"
            "@param $2: The virtual device address of the NIC being allocated\n"
            "@param $3: The type of NIC which is to be allocated\n"
            "@param $4: The number of VDEV channels associated with this NIC\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC != 5) {
        printf("Error: Wrong number of parameters");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get inputs
    image = argV[1];
    address = argV[2];
    type = argV[3];
    devcount = atoi(argV[4]);

    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }


    if (!strcmp(type, "HiperSockets")) {
        typecode = 1;
    } else if (!strcmp(type, "QDIO")) {
        typecode = 2;
    } else {
        printf("Error: NIC type must be HiperSockets or QDIO\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    printf("Adding NIC %s to %s... ", address, image);
    rc = smVirtual_Network_Adapter_Create_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            image, address, typecode, devcount, "",  // Only relevant for a second-level z/OS.
            "",  // The MAC identifier.
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
