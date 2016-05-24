/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliDelete.h"
#include "wrapperutils.h"

int deleteABENDDump(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    // Parse the command-line arguments
    int option;
    int rc;
    char * image = NULL;
    int entryCount = 0;
    int arrayBytes = 0;
    char ** entryArray = NULL;
    const char * optString = "-T:k:h?";
    vmApiDeleteABENDDumpOutput* output;

    // Count up the max number of arguments to create the array
    opterr = 0;
    while ((option = getopt(argC, argV, optString)) != -1) {
        arrayBytes = arrayBytes + sizeof(* entryArray);
    }

    optind = 1;  // Reset optind so getopt can rescan arguments
    opterr = 1;

    if (arrayBytes > 0) {
        if (0 == (entryArray = malloc(arrayBytes)))return MEMORY_ERROR;
    }

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, optString)) != -1) {
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'k':
                entryArray[entryCount] = optarg;
                entryCount++;
                break;

            case 'h':
                printf("NAME\n"
                    "  Delete_ABEND_Dump\n\n"
                    "SYNOPSIS\n"
                    "  smcli Delete_ABEND_Dump [-T] image_name\n"
                    "   [-k] 'entry1' \n"
                    "DESCRIPTION\n"
                    "  Use Delete_ABEND_Dump to instruct the dump processing userid to remove \n"
                    "  a specified ABEND dump from the reader or from the dump processing location \n"
                    "  The following options are required:\n"
                    "    -T    The name of the virtual machine with authorization to delete the dump.\n"
                    "    -k    A quoted keyword=value 'id=value' \n"
                    "          The value is a filename (SFS directory) or \n"
                    "          spool ID (reader) of a dump \n");
                FREE_MEMORY_CLEAR_POINTER(entryArray);
                return 1;
                break;

            case '?':  // Missing option data!
                  FREE_MEMORY_CLEAR_POINTER(entryArray);
                  return 1;
                  break;

            case 1:  // API name type data(other non option element key data)
                break;

            default:
                FREE_MEMORY_CLEAR_POINTER(entryArray);
                return 1;
                break;
        }
     }

     if (!image || !entryCount || strncmp("id=", entryArray[0], 3) !=0) {
       printf("\nERROR: Missing required options\n");
       if (arrayBytes > 0){
           FREE_MEMORY_CLEAR_POINTER(entryArray);
       }
       return 1;
    }

    printf("Deleting ABEND Dump for %s ...", entryArray[0]);
    rc = smDelete_ABEND_Dump(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
         image, entryCount, entryArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Delete_ABEND_Dump", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Delete_ABEND_Dump", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }

    FREE_MEMORY_CLEAR_POINTER(entryArray);
    return rc;
}
