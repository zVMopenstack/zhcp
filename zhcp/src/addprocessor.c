/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Adds a non-dedicated processor to a virtual server's directory entry.
 *
 * @param $1: The guest to which a processor is to be added
 * @param $2: The virtual device address for the processor being added
 *
 * @return 0 If the processor was created
 *         1 If given invalid parameters
 *         2 If the processor definition failed
 */
int main(int argC, char * argV[]) {
    int rc;
    char* image = NULL;
    char* address = NULL;
    vmApiImageCpuDefineDmOutput* output;
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
        printf("Adds a non-dedicated processor to a virtual server's directory entry.\n\n"
            "Usage: addprocessor [@params]\n"
            "@param $1: The guest to which a processor is to be added\n"
            "@param $2: The virtual device address for the processor being added\n");
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
    address = argV[2];  // Hexedecimal string, 00-3F
    printf("Adding processor %s to %s... ", address, image);
    rc = smImage_CPU_Define_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
             image, address, 0,  // Base CPU status unspecified.
             "",  // The processor identification number.
             1,  // Not a dedicated processor.
             0,  // Do not specify a cryptographic facility.
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
