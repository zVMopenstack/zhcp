/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Creates a virtual server with the specified name.
 *
 * @param $1: The name of the virtual server which is to be created
 * @param $2: Text file containing the user directory entry
 *
 * @return 0 If the image now exists
 *         1 If given invalid parameters
 *         2 If image creation failed
 */
int main(int argC, char * argV[]) {
    int rc;
    char * imageName = NULL;
    char * file = NULL;
    vmApiImageCreateDmOutput * output;
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
        printf("Creates a virtual server with the specified name.\n\n"
            "Usage: createvs [@params]\n"
            "@param $1: The name of the virtual server which is to be created\n"
            "@param $2: Text file containing the user directory entry\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC > 3) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get inputs
    imageName = argV[1];
    file = argV[2];
    // Check if image name is between 1 and 8 characters
    if (isImageNameInvalid(imageName)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    printf("Creating user directory entry for %s... ", imageName);
    // Open the user entry file
    FILE * fp = fopen(file, "r");
    if (fp == NULL) {
        printf("  Error: Failed to open %s\n", file);
        rc = 2;
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

    rc = smImage_Create_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password
                imageName, "",
                0, "",  // Initial password length, initial password
                "", recordCount,  // Initial account number, image record array length
                (vmApiImageRecord *) record,  // Image record
                &output);

    if (rc || (output->common.returnCode && output->common.returnCode != 400 && output->common.returnCode != 592)
            || (output->common.reasonCode && output->common.reasonCode != 8 && output->common.reasonCode != 0)) {
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
