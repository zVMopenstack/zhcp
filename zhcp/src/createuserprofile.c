/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Creates a profile directory entry with the specified name.
 *
 * @param $1: Name of the profile which is to be created
 * @param $2: Text file containing the profile directory entry
 *
 * @return 0 If user profile entry was successfully created
 *         1 If given invalid parameters
 *         2 If user profile creation failed
 */
int main(int argC, char* argV[]) {
    int rc;
    char* profileName = NULL;
    char* file = NULL;
    vmApiProfileCreateDmOutput* output;
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
        printf("Creates a profile directory entry with the specified name.\n\n"
            "Usage: createuserprofile [@params]\n"
            "@param $1: Name of the profile which is to be created\n"
            "@param $2: Text file containing the profile directory entry\n");
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
    profileName = argV[1];
    file = argV[2];

    // Check if the user profile name is between 1 and 8 characters
    if (isProfileNameInvalid(profileName)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Open the user entry file
    FILE * fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Error: Failed to open %s\n", file);
        rc = 2;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    printf("Creating user profile %s... ", profileName);

    // Count the number of lines and set the record count to it
    int recordCount = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n')
            recordCount++;
    }

    // Reset position to start of file
    rewind(fp);

    // Create image record
    vmApiProfileRecord record[recordCount];
    int i = 0, LINE_SIZE = 72;
    char line[recordCount][LINE_SIZE];
    char * ptr;
    while (fgets(line[i], LINE_SIZE, fp) != NULL) {
        // Replace newline with null terminator
        ptr = strstr(line[i], "\n");
        if (ptr != NULL)
            strncpy(ptr, "\0", 1);

        record[i].profileRecordLength = strlen(line[i]);
        record[i].recordData = line[i];
        i++;
    }

    // Close file
    fclose(fp);

    rc = smProfile_Create_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password
                profileName, recordCount,
                (vmApiProfileRecord *) record,  // Image record
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
