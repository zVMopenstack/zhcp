/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Log off the specified virtual server.
 *
 * @param $1: The name of the guest which is to be logged off
 *
 * @return 0 If the image is powered off upon completion
 *         1 If given invalid parameters
 *         2 If power-off failed
 */
int main(int argC, char* argV[]) {
    int rc;
    char *imageName;
    vmApiImageDeactivateOutput* output;
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
        printf("Log off the specified virtual server.\n\n"
            "Usage: stopvs [@params]\n"
            "@param $1: The name of the guest which is to be logged off\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC != 2) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get the virtual server name
    imageName = argV[1];
    // Check if the virtual server name is between 1 and 8 characters
    if (isImageNameInvalid(imageName))
        return 1;

    printf("Stopping %s... ", imageName);

    // Log off virtual server
    rc = smImage_Deactivate(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            imageName, "IMMED",  // Immediate deactivation.
            &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 200)
            || (output->common.reasonCode && output->common.reasonCode != 12)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
            "  Reason Code: %d\n", output->common.returnCode,
                output->common.reasonCode);
    } else {
        printf("Done\n");
    }

    TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
    /* Clean up for memory context */
    FREE_CONTEXT_MEMORY(&vmapiContext);
    return rc;
}
