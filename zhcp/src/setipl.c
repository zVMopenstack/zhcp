/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Sets the IPL statement in a specified virtual server's directory entry.
 *
 * @param $1: The name of the guest for which the IPL statement is to be set
 * @param $2: The new IPL statement's IPL target string
 * @param $3: The new IPL statement's load parameter string
 * @param $4: The new IPL statement's parameter string
 *
 * @return 0 If the virtual image now exists
 *         1 If given invalid parameters
 *         2 If image creation failed
 */
int main(int argC, char * argV[]) {
    int rc;
    int i = 0;
    char* image = NULL;
    char* ipltarget = NULL;
    char* loadparms = "";
    char parms[64] = "";
    vmApiImageIplSetDmOutput* output;
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
        printf("Sets the IPL statement in a specified virtual server's directory entry.\n\n"
            "Usage: setipl [@params]\n"
            "@param $1: The name of the guest for which the IPL statement is to be set\n"
            "@param $2: The new IPL statement's IPL target string\n"
            "@param $3: The new IPL statement's load parameter string\n"
            "@param $4: The new IPL statement's parameter string\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC < 3) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }
    // Get inputs
    image = argV[1];
    ipltarget = argV[2];

    if (argC > 3 && argV[3])
        loadparms = argV[3];
    if (argC > 4 && argV[4]) {
        // Create a string
        for (i = 4; i < argC; i++) {
            strcat(parms, argV[i]);
            strcat(parms, " ");
        }
    }

    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    printf("Setting IPL statement on %s... ", image);
    rc = smImage_IPL_Set_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
              image, ipltarget, loadparms, parms, &output);

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
