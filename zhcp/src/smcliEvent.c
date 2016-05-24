/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliEvent.h"
#include "wrapperutils.h"

int eventStreamAdd(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    // Parse the command-line arguments
    int option;
    int rc;
    long eventId;
    int eventIdSpecified = 0;
    char * image = NULL;
    int stringCount = 0;
    char ** stringArray = 0;
    int hexstringCount = 0;  // Allow hex character input some day?
    char ** hexstringArray;
    char * end;
    int arrayBytes = 0;
    const char * optString = "-T:d:e:h?";
    vmApiEventStreamAddOutput* output;

    // Count up the max number of arguments to create the array
    opterr = 0;  // Supress messages
    while ((option = getopt(argC, argV, optString)) != -1) {
        arrayBytes = arrayBytes + sizeof(*stringArray);
    }

    optind = 1;  // Reset optind so getopt can rescan arguments
    opterr = 1;  // Enable messages

    if (arrayBytes > 0) {
        if (0 == (stringArray = malloc(arrayBytes)))return MEMORY_ERROR;
    }

    // Options that have arguments are followed by a : character
    // Future enhancement is to allow hex char string input x:
    // Add - to getopt to force processing in order of options specified
    while ((option = getopt(argC, argV, "-T:d:e:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'd':
                stringArray[stringCount] = optarg;
                stringCount++;
                break;

            case 'e':
                eventIdSpecified = 1;
                eventId = strtol( optarg, &end, 10);
                if ( *end != '\0' ) {
                    printf( "\nERROR: Event Id must be an integer > 16777215\n" );
                    return 1;
                }
                break;

            case 'h':
                printf("NAME\n"
                    "  Event_Stream_Add\n\n"
                    "SYNOPSIS\n"
                    "  smcli Event_Stream_Add [-T] image_name [-e] event_type \n"
                    "   [-d] 'event data'\n"
                    "DESCRIPTION\n"
                    "  Use Event_Stream_Add to put type 1 event data on *VMEVENT stream\n"
                    "  The following options are required:\n"
                    "    -T    The name of the authorized virtual machine for this API.\n"
                    "    -e    An integer id value > 16777215\n"
                    "    -d    quoted string containing the event data\n");
                FREE_MEMORY_CLEAR_POINTER(stringArray);
                return 1;
                break;

            case '?':  // Missing option data or invalid option
                FREE_MEMORY_CLEAR_POINTER(stringArray);
                return 1;
                break;

            case 1:  // API name type data(other non option element key data)
                break;

            default:
                FREE_MEMORY_CLEAR_POINTER(stringArray);
                return 1;
                break;
     }

     if (!image || ( !eventIdSpecified ) || !stringCount) {
         printf("\nERROR: Missing required options\n");
         if (arrayBytes > 0){
             FREE_MEMORY_CLEAR_POINTER(stringArray);
         }
         return 1;
     }

     if (( eventId < 16777215)) {
         printf( "\nERROR: Event Id must be an integer greater than 16777215\n" );
         if (arrayBytes > 0){
             FREE_MEMORY_CLEAR_POINTER(stringArray);
         };
         return 1;
     }

    printf("Adding event id %d event data.", eventId);
    rc = smEvent_Stream_Add(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
         image, (int) eventId, stringCount, stringArray, hexstringCount, hexstringArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Event_Stream_Add", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Event_Stream_Add", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int eventSubscribe(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    // Skip doing this since it requires a continuously open socket
    int rc = 0;
    printf("Not implemented.\n");
    return rc;
}

int eventUnsubscribe(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    // Skip doing this since we do not support subscribe. (Requires a continuously open socket)
    int option;
    int rc = 0;
    printf("Not implemented.\n");
    return rc;
}
