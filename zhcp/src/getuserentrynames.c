/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include <stdlib.h>
#include "wrapperutils.h"

/**
 * Prints out a list of defined image names on a z/VM system.
 *
 * @return 0 If the list of image names was successfully printed
 *         1 If given invalid parameters
 *         2 If the image name list could not be retrieved
 */
int main(int argC, char * argV[]) {
    int rc;
    vmApiImageNameQueryDmOutput* output;
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

    if (argV[1] && (!strcmp(argV[1], "-h") || !strcmp(argV[1], "--help"))) {
        printf("Prints out a list of defined image names on a z/VM system.\n\n"
            "Usage: getuserentrynames\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
       return rc;
    } else if (argC > 1) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    rc = smImage_Name_Query_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            "NA",  // Doesn't matter what image name we use.
            &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 0)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
            "  Reason Code: %d\n", output->common.returnCode, output->common.reasonCode);
    } else {
        int i;
        int n = output->nameCount;
        for (i = 0; i < n; i++) {
            printf("%s\n", output->nameList[i]);
        }
    }

    TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
    /* Clean up for memory context */
    FREE_CONTEXT_MEMORY(&vmapiContext);
    return rc;
}
