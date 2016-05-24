/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Deletes the specified user profile.
 *
 * @param $1: The name of the user profile which is to be deleted
 *
 * @return 0 If the user profile is not present in the directory upon completion
 *         1 If given invalid parameters
 *         2 If user profile deletion failed
 */
int main(int argC, char* argV[]) {
    int rc;
    char* profileName = NULL;
    vmApiProfileDeleteDmOutput* output;
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
        printf("Deletes the specified user profile.\n\n"
            "Usage: deleteuserprofile [@params]\n"
            "@param $1: The name of the user profile which is to be deleted\n");
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

    // Get the user profile name
    profileName = argV[1];
    // Check if the user profile name is between 1 and 8 characters
    if (isProfileNameInvalid(profileName)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }
    printf("Deleting user profile %s... ", profileName);
    // Delete user profile
    rc = smProfile_Delete_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password
            profileName,  // Profile name
            &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 400
            && output->common.returnCode != 592) || (output->common.reasonCode
            && output->common.reasonCode != 4 && output->common.reasonCode != 0)) {
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

