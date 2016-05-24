/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * Adds a 9336 (FBA) disk to a virtual server's directory entry.
 *
 * @param $1: The name of the guest to which a disk is to be added
 * @param $2: The name of the disk pool from which a disk is to be allocated
 * @param $3: The virtual device address for the newly-allocated volume
 * @param $4: Block size (512, 1024, 2048, or 4096)
 * @param $5: The size of the new disk, in blocks
 * @param $6: The mode under which the disk is to be linked by the user
 * @param $7: The read password
 * @param $8: The write password
 * @param $9: The multi password
 *
 * @return 0 If the disk was added successfully
 *         1 If given invalid parameters
 *         2 If disk allocation failed
 */
int main(int argC, char* argV[]) {
    int rc;
    int unit;
    int blocksize = 0;
    int size = 0;
    char* image = NULL;
    char* pool = NULL;
    char* vdev = NULL;
    char* mode = NULL;
    vmApiImageDiskCreateDmOutput* output;
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
        printf("Adds a 9336 (FBA) disk to a virtual server's directory entry.\n\n"
            "Usage: add9336 [@params]\n"
            "@param $1: The name of the guest to which a disk is to be added\n"
            "@param $2: The name of the disk pool from which a disk is to be allocated\n"
            "@param $3: The virtual device address for the newly-allocated volume\n"
            "@param $4: Block size (512, 1024, 2048, or 4096)\n"
            "@param $5: The size of the new disk, in blocks\n"
            "@param $6: The mode under which the disk is to be linked by the user\n"
            "@param $7: The read password\n"
            "@param $8: The write password\n"
            "@param $9: The multi password\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    } else if (argC < 7 || argC > 10) {
        printf("Error: Wrong number of parameters\n");
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    // Get inputs
    image = argV[1];
    pool = argV[2];
    vdev = argV[3];
    blocksize = atoi(argV[4]);
    size = atoi(argV[5]);
    mode = argV[6];

    // Read password
    char* readPw = "";
    if (argC > 8 && argV[7])
        readPw = argV[7];

    // Write password
    char* writePw = "";
    if (argC > 9 && argV[8])
        writePw = argV[8];

    // Mutli password
    // In order to link to a disk, a multi password must be specified
    char* multiPw = "";
    if (argC > 10 && argV[9])
        multiPw = argV[9];

    // Check if the userID is between 1 and 8 characters
    if (isImageNameInvalid(image)) {
        rc = 1;
        TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
        /* Clean up for memory context */
        FREE_CONTEXT_MEMORY(&vmapiContext);
        return rc;
    }

    switch (blocksize) {
        case 512:
            unit = 2;
            break;
        case 1024:
            unit = 3;
            break;
        case 2048:
            unit = 4;
            break;
        case 4096:
            unit = 5;
            break;
        default:
            printf("Error: Block size must be 512, 1024, 2048, or 4096\n");
            rc = 1;
            TRACE_EXIT_FLOW(&vmapiContext, TRACEAREA_ZHCP_GENERAL, rc);
            /* Clean up for memory context */
            FREE_CONTEXT_MEMORY(&vmapiContext);
            return rc;
    }

    printf("Adding disk %s to %s(config)... ", vdev, image);
    rc = smImage_Disk_Create_DM(&vmapiContext, "", 0, "",  // Authorizing user, password length, password.
            image, vdev, "9336", "AUTOG", pool, unit, size, mode, 1, "",  // Do not format disk.  Do not give disk a label.
            readPw, writePw, multiPw,  // Read, write, and multi passwords.
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
