/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Replace a specified directory entry.
 *
 * @param $1: Name of the guest which is to be replace
 * @param $2: Text file containing the new directory entry
 *
 * @return 0 If directory entry was successfully replaced
 *         1 If given invalid parameters
 *         2 If directory entry replacement failed
 */
int main(int argC, char* argV[]) {
    int rc;
    char* image = NULL;
    char* file = NULL;
    vmApiImageReplaceDmOutput* output;
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
        printf("Replace a specified directory entry.\n\n"
            "Usage: replacevs [@params]\n"
            "@param $1: Name of the guest which is to be replace\n"
            "@param $2: Text file containing the new directory entry\n");
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
    file = argV[2];

    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image)) {
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

    // Lock user entry
    rc = lockImage(image, &vmapiContext);
    if (rc) {
        printf("Error: Failed to lock the image\n");
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

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
    vmApiImageRecord record[recordCount];
    int i = 0, LINE_SIZE = 72;
    char line[recordCount][LINE_SIZE];
    char * ptr;
    while (fgets(line[i], LINE_SIZE, fp) != NULL) {
        // Replace newline with null terminator
        ptr = strstr(line[i], "\n");
        if (ptr != NULL) {
            strncpy(ptr, "\0", 1);
        }
        record[i].imageRecordLength = strlen(line[i]);
        record[i].imageRecord = line[i];
        i++;
    }

    // Close file
    fclose(fp);

    printf("Replacing user entry of %s... ", image);
    rc = smImage_Replace_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password
                image, recordCount,
                (vmApiImageRecord *) record,  // Image record
                &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 0)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
                    "  Reason Code: %d\n", output->common.returnCode, output->common.reasonCode);

        // Unlock user entry
        rc = unlockImage(image, vmapiContext);
    } else {
        printf("Done\n");
    }

    TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
    /* Clean up for memory context */
    FREE_CONTEXT_MEMORY(&vmapiContext);
    return rc;
}

/**
 * Lock a specified user entry.
 *
 * @param $1: Name of the virtual server which is to be locked
 *
 * @return 0 If user entry was successfully locked
 *         1 If given invalid parameters
 *         2 If user entry lock failed
 */
int lockImage(char * image, vmApiInternalContext context) {
    printf("Locking %s... ", image);

    vmApiImageLockDmOutput * output;
    int rc = smImage_Lock_DM(&context, "",  // Authorizing user
            0,  // Password length
            "",  // Password
            image,  // Image name
            "",  // Device virtual address
            &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 400)
            || (output->common.reasonCode && output->common.reasonCode != 12)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
                    "  Reason Code: %d\n", output->common.returnCode, output->common.reasonCode);
        return 2;
    } else {
        printf("Done\n");
        return 0;
    }
}

/**
 * Unlock a specified user entry.
 *
 * @param $1: Name of the virtual server which is to be unlocked
 *
 * @return 0 If user entry was successfully unlocked
 *         1 If given invalid parameters
 *         2 If user entry unlock failed
 */
int unlockImage(char * image, vmApiInternalContext context) {
    printf("Unlocking %s... ", image);

    vmApiImageUnlockDmOutput * output;
    int rc = smImage_Unlock_DM(&context, "",  // Authorizing user
            0,  // Password length
            "",  // Password
            image,  // Image name
            "",  // Device virtual address
            &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 0)) {
        printf("Failed\n");

        rc ? printf("  Return Code: %d\n", rc) : printf("  Return Code: %d\n"
                    "  Reason Code: %d\n", output->common.returnCode, output->common.reasonCode);
        return 2;
    } else {
        printf("Done\n");
        return 0;
    }
}
