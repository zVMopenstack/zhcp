/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Prints out information about disk spaces on a z/VM system.
 *
 * @param $1: The type of query to run
 * @param $2: The type of space to query
 *
 * @return 0 If the virtual image was created
 *         1 If given invalid parameters
 *         2 If the image name list could not be retrieved
 */
int main(int argC, char * argV[]) {
    int rc;
    vmApiImageVolumeSpaceQueryDmOutput* output;
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
        printf("Prints out information about disk spaces on a z/VM system.\n\n"
            "Usage: queryvolumes [@params]\n"
            "@param $1: The type of query to run, where the value can be either:\n"
            "  definition\n"
            "  free\n"
            "  used\n"
            "@param $2: The type of space to query, where the value can be either:\n"
            "  volume\n"
            "  region\n"
            "  group\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC != 3) {
        rc = 1;
        printf("Error: Wrong number of parameters\n");
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get inputs
    char* queryType = argV[1];
    char* entryType = argV[2];

    int iQueryType;
    if (!strcmp(queryType, "definition")) {
        iQueryType = 1;
    } else if (!strcmp(queryType, "free")) {
        iQueryType = 2;
    } else if (!strcmp(queryType, "used")) {
        iQueryType = 3;
    } else {
        rc = 1;
        printf("Error: Invalid query type\n");
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    int iEntryType;
    if (!strcmp(entryType, "volume")) {
        iEntryType = 1;
    } else if (!strcmp(entryType, "region")) {
        iEntryType = 2;
    } else if (!strcmp(entryType, "group")) {
        iEntryType = 3;
    } else {
        rc = 1;
        printf("Error: Invalid entry type\n");
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    rc = smImage_Volume_Space_Query_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            "NA",  // Doesn't matter what image name we use.
            iQueryType, iEntryType, "*",  // List all spaces of specified type.
            &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 0)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
            "  Reason Code: %d\n", output->common.returnCode, output->common.reasonCode);
    } else {
        int i, l;
        int n = output->recordCount;
        int p = 0;
        for (i = 0; i < n; i++) {
            l = output->recordList[i].imageRecordLength;
            printf("%.*s\n", l, output->recordList[0].imageRecord + p);
            p = p + l;
        }
    }

    TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
    /* Clean up for memory context */
    FREE_CONTEXT_MEMORY(&vmapiContext);
    return rc;
}
