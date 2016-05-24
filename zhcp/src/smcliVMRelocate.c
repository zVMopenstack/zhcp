/**
f 1 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliVMRelocate.h"
#include "wrapperutils.h"


int vmRelocate(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int entryCount = 0;
    int option;
    int destSpecified = 0;
    char * targetIdentifier = NULL;
    char * entryArray[6];
    vmApiVMRelocateOutput * output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:k:h?")) != -1)
        switch (option) {
            case 'T':
                targetIdentifier = optarg;
                break;

            case 'k':
                if (!optarg) {
                    return INVALID_DATA;
                }
                if (entryCount < 6) {
                    entryArray[entryCount] = optarg;
                    if (( strlen(optarg) > 12) && (strncmp( "destination=", optarg, 12 ) == 0 )) {
                    	destSpecified = 1;
                    }
                    entryCount++;
                } else {
                    printf(" Error Too many -k values entered.\n");
                    return INVALID_DATA;
                }

                break;

            case 'h':
                printf("NAME\n"
                    "  VMRELOCATE\n\n"
                    "SYNOPSIS\n"
                    "  smcli VMRELOCATE [-T] targetIdentifier\n"
                    "  [-k] entry1 [-k] entry2 ...\n\n"
                    "DESCRIPTION\n"
                    "  Use VMRELOCATE to relocate, test relocation eligibility, or cancel the \n"
                    "  relocation of the specified virtual machine, while it continues to run,\n"
                    "  to the specified system  within the z/VM SSI cluster.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of the virtual machine whose relocation to another system\n"
                    "          within the z/VM SSI cluster will be initiated, tested, or canceled.\n"
                    "    -k    A keyword=value item to be created in the directory.\n"
                    "          They may be specified in any order. Possible keywords are:\n"
                    "              destination: The z/VM SSI cluster name of the destination system\n"
                    "                           to which the specified virtual machine will be\n"
                    "                           relocated.\n"
                    "                  * Note that this parameter is not used when canceling a\n"
                    "                    relocation. If it is specified with 'action=CANCEL', it\n"
                    "                    will be ignored.\n\n"
                    "              action: One of the following:\n"
                    "                MOVE - Initiate a VMRELOCATE MOVE of the virtual machine\n"
                    "                       specified in target_identifier.\n"
                    "                TEST - Test the specified virtual machine and determine if it\n"
                    "                       is eligible to be relocated to the specified system. If\n"
                    "                       TEST is specified, all other valid additional input\n"
                    "                       parameters except destination = are ignored.\n"
                    "                       If action = is not, specified TEST is the default.\n\n"
                    "                CANCEL - Stop the relocation of the specified virtual machine.\n"
                    "                         If CANCEL is specified, all other additional input\n"
                    "                         parameter are ignored.\n\n"
                    "              force: Any combination of the following may be specified,\n"
                    "                     in any order:\n"
                    "                ARCHITECTURE - Indicates that relocation is to be attempted\n"
                    "                               even though the virtual machine is currently\n"
                    "                               running on a system with hardware architecture\n"
                    "                               facilities or CP-supplied features not\n"
                    "                               available on the destination system\n"
                    "                               For example, when relocating to a system\n"
                    "                               running an earlier release of CP.\n"
                    "                DOMAIN - Indicates that relocation is to be attempted\n"
                    "                         even though the virtual machine would be moved\n"
                    "                         outside of its domain.\n"
                    "                STORAGE - Indicates that relocation should proceed even\n"
                    "                          if CP determines that there are insufficient\n"
                    "                          storage resources available on the destination,\n"
                    "                          following memory capacity assessment checks.\n"
                    "                  For example, to choose all three options,specify\n"
                    "                  'force=ARCHITECTURE DOMAIN STORAGE'.\n\n"
                    "              immediate:  One of the following:\n"
                    "                NO -  Specifies immediate processing. This is the default.\n"
                    "                YES - The VMRELOCATE command will do one early pass through\n"
                    "                      virtual machine storage and then go directly to the\n"
                    "                      quiesce stage. The defaults for both max_total and\n"
                    "                      max_quiesce are NOLIMIT when immediate=YES is specified\n\n"
                    "              max_total:  One of the following:\n"
                    "                NOLIMIT - Specifies that there is no limit on the total amount\n"
                    "                          of time the system should allow for this relocation.\n"
                    "                          The relocation will therefore not be canceled due to\n"
                    "                          time constraints. This is the default.\n"
                    "                value - The maximum total time (in seconds) that the command\n"
                    "                        issuer is willing to wait for the entire relocation\n\n"
                    "              max_quiesce: One of the following:\n"
                    "                NOLIMIT - Specifies that there is no limit on the total\n"
                    "                          quiesce time the system should allow for this\n"
                    "                          relocation.\n"
                    "                value - The maximum quiesce time(in seconds)a virtual machine\n"
                    "                        may be stopped during a relocation attempt. The range\n"
                    "                        for this  value is 1-99999999. The default is NOLIMIT\n"
                    "                        if immediate=YES is specified, or 10 seconds if not.\n\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if ( !targetIdentifier || !destSpecified )  {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    for (i = 0; i < entryCount; i++) {
    	if ((strncmp(entryArray[i], "A", 1) == 0) || (strncmp(entryArray[i], "a", 1) == 0))  {
            printf("Running VMRELOCATE %s against %s... ", entryArray[i], targetIdentifier);
            break;
    	}
    }
    rc = smVMRELOCATE(vmapiContextP, "", 0, "", targetIdentifier, entryCount, entryArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("VMRELOCATE", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescriptionAndErrorBuffer("VMRELOCATE", rc,
                output->common.returnCode, output->common.reasonCode, output->errorDataLength, output->errorData,
                vmapiContextP);
    }
    return rc;
}

int vmRelocateImageAttributes(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int entryCount = 0;
    int option;
    char * targetIdentifier = NULL;
    char * entryArray[3];
    vmApiVMRelocateImageAttributesOutput * output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:k:h?")) != -1)
        switch (option) {
            case 'T':
                targetIdentifier = optarg;
                break;

            case 'k':
                if (!optarg) {
                    return INVALID_DATA;
                }
                if (entryCount < 3) {
                    entryArray[entryCount] = optarg;
                    entryCount++;
                } else {
                    printf(" Error Too many -k values entered.\n");
                    return INVALID_DATA;
                }
                break;

            case 'h':
                printf("NAME\n"
                    "  VMRELOCATE_Image_Attributes\n\n"
                    "SYNOPSIS\n"
                    "  smcli VMRELOCATE_Image_Attributes [-T] targetIdentifier\n"
                    "  [-k] 'entry1' [-k] 'entry2' ...\n\n"
                    "DESCRIPTION\n"
                    "  Use VMRELOCATE_Image_Attributes to modify the relocation setting for a\n"
                    "  the specified virtual machine, while it continues to run,\n"
                    "  to the specified system  within the z/VM SSI cluster.\n\n"
                    "  The following options are required:\n"
                    "    -T    The user ID whose relocation capability is being set. If * is\n"
                    "          specified, the target user is the command issuer.\n"
                    "    -k    A keyword=value item to be created in the directory.\n"
                    "          They may be specified in any order. They must be inside\n"
                    "          single quotes. For example -k 'key=value'\n. Possible keywords are:\n"
                    "              relocation_setting - One of the following:\n"
                    "                  ON - Enables relocation for the specified user.\n"
                    "                  OFF - Disables relocation for the specified user.\n"
                    "              domain_name - The domain currently associated with a user.\n"
                    "                            If unspecified, the currently associated domain\n"
                    "                            is assumed\n"
                    "              archforce -  One of the following:\n"
                    "                  NO - The guest's virtual machine will not be set to a new\n"
                    "                       domain.\n"
                    "                 YES - Specifies the FORCE ARCHITECTURE option, in which the\n"
                    "                       virtual machine is assigned to the new domain even if\n"
                    "                       it means the guest's virtual architecture will be set to\n"
                    "                       a level with less capability than it had in its original\n"
                    "                       domain. If unspecified, the default is NO.\n\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!targetIdentifier ||  entryCount < 1)  {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Modifying the relocation setting for %s... ", targetIdentifier);
    rc = smVMRELOCATE_Image_Attributes(vmapiContextP, "", 0, "",  targetIdentifier, entryCount, entryArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("VMRELOCATE_Image_Attributes", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("VMRELOCATE_Image_Attributes", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int vmRelocateModify(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int entryCount = 0;
    int option;
    char * targetIdentifier = NULL;
    char * entryArray[2];
    vmApiVMRelocateModifyOutput * output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:k:h?")) != -1)
        switch (option) {
            case 'T':
                targetIdentifier = optarg;
                break;

            case 'k':
                if (!optarg) {
                    return INVALID_DATA;
                }
                if (entryCount < 2) {
                    entryArray[entryCount] = optarg;
                    entryCount++;
                } else {
                    printf(" Error Too many -k values entered.\n");
                    return INVALID_DATA;
                }
                break;

            case 'h':
                printf("NAME\n"
                    "  VMRELOCATE_Modify\n\n"
                    "SYNOPSIS\n"
                    "  smcli VMRELOCATE_Modify [-T] targetIdentifier\n"
                    "  [-k] entry1 [-k] entry2 ...\n\n"
                    "DESCRIPTION\n"
                    "  Use VMRELOCATE_Modify to modify the time limits associated with a relocation\n"
                    "  already in progress for the specified image\n"
                    "  The following options are required:\n"
                    "    -T    The name of the virtual machine whose relocation to another system\n"
                    "          within the z/VM SSI cluster is already in the process of relocation\n"
                    "          for which the time limits should be modified.\n"
                    "    -k    A keyword=value item to be created in the directory.\n"
                    "          They may be specified in any order. Possible keywords:\n"
                    "            max_total: One of the following:\n"
                    "              NOLIMIT - Indicates that there is no limit on the total amount\n"
                    "                        of time the system should allow for this relocation.\n"
                    "                        The relocation will therefore not be canceled due to\n"
                    "                        time constraints. This is the default if unspecified.\n"
                    "              value - The maximum total time (in seconds) that the command\n"
                    "                      issuer is willing to wait for the entire relocation\n"
                    "                      to complete.\n"
                    "            max_quiesce: One of the following:\n"
                    "              NOLIMIT - Indicates that there is no limit on the total\n"
                    "                        quiesce time the system should allow for this\n"
                    "                        relocation.\n"
                    "              value - The maximum quiesce time in seconds a virtual machine\n"
                    "                      may be stopped during a relocation attempt.The default\n"
                    "                      if unspecified is 10 seconds.The range for this value\n"
                    "                      is 1-99999999.\n\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!targetIdentifier)  {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Modifying the time limits associated with a relocation for %s... ", targetIdentifier);
    rc = smVMRELOCATE_Modify(vmapiContextP, "", 0, "",  targetIdentifier, entryCount, entryArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("VMRELOCATE_Modify", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code and Error Buffer if it was sent
        rc = printAndLogSmapiReturnCodeReasonCodeDescriptionAndErrorBuffer("VMRELOCATE_Modify", rc,
                output->common.returnCode, output->common.reasonCode, output->errorDataLength, output->errorData,
                vmapiContextP);
    }
    return rc;
}

int vmRelocateStatus(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int entryCount = 0;
    int option;
    char * targetIdentifier = NULL;
    char * entryArray[1];
    char *token;
    char * buffer;  // Character pointer whose value is preserved between successive related calls to strtok_r
    char * blank = " ";
    char vmRelocate_image[8 + 1];
    char vmRelocate_source_system[8 + 1];
    char vmRelocate_destination_system[8 + 1];
    char vmRelocate_by[8 + 1];
    char vmRelocate_elapsed[8 + 1];
    char vmRelocate_status[15 + 1];
    vmApiVMRelocateStatusOutput * output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:k:h?")) != -1)
        switch (option) {
            case 'T':
                targetIdentifier = optarg;
                break;

            case 'k':
                if (!optarg) {
                    return INVALID_DATA;
                }
                if (entryCount < 1) {
                    entryArray[entryCount] = optarg;
                    entryCount++;
                } else {
                    printf("ERROR: Too many -k values entered\n");
                    return INVALID_DATA;
                }
                break;

            case 'h':
                printf("NAME\n"
                    "  VMRELOCATE_Status\n\n"
                    "SYNOPSIS\n"
                    "  smcli VMRELOCATE_Status [-T] targetIdentifier\n"
                    "  [-k] entry1 [-k] entry2 ...\n\n"
                    "DESCRIPTION\n"
                    "  Use VMRELOCATE_Status to obtain information about relocations currently in\n"
                    "  progress.\n\n"
                    "  The following options are required:\n"
                    "    -T    The name of a virtual machine whose relocation to another system\n"
                    "          within the z/VM SSI cluster that we want the status of.\n"
                    "    -k    A keyword=value item to be created in the directory.\n"
                    "           They may be specified in any order. Possible keywords are:\n"
                    "              status_target - One of the following:\n"
                    "                  ALL - Specifies that the status of all relocations currently\n"
                    "                        in progress on this system are displayed.\n"
                    "                  USER userid - Display relocation status of the virtual\n"
                    "                                machine with name userid.\n"
                    "                  INCOMING - Display status of all incoming relocations.\n"
                    "                  OUTGOING - Display status of all outgoing relocations.\n"
                    "              If unspecified, ALL is the default.\n\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!targetIdentifier)  {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    rc = smVMRELOCATE_Status(vmapiContextP, "", 0, "",  targetIdentifier, entryCount, entryArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("VMRELOCATE_Status", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("VMRELOCATE_Status", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        // Obtain the 6 members of the VMRELOCATE_status_structure
        for (i = 0; i < output->statusArrayCount; i++) {
            // Obtain VMRELOCATE_image
            token = strtok_r(output->statusArray[i].vmapiString, blank, &buffer);
            if (token != NULL) {
                strcpy(vmRelocate_image, token);
            } else  {
                printf("ERROR: VMRELOCATE_image is NULL\n");
                rc =1;
                break;
            }

            // Obtain VMRELOCATE_source_system
            token = strtok_r(NULL, blank, &buffer);
            if (token != NULL) {
                strcpy(vmRelocate_source_system, token);
            } else {
                printf("ERROR: VMRELOCATE_source_system is NULL\n");
                rc =1;
                break;
            }
            // Obtain VMRELOCATE_destination_system
            token = strtok_r(NULL, blank, &buffer);
            if (token != NULL) {
                strcpy(vmRelocate_destination_system, token);
            } else {
                printf("ERROR: VMRELOCATE_destination_system is NULL\n");
                rc =1;
                break;
            }
            // Obtain VMRELOCATE_by
            token = strtok_r(NULL, blank, &buffer);
            if (token != NULL) {
                strcpy(vmRelocate_by, token);
            } else {
                printf("ERROR: VMRELOCATE_by is NULL\n");
                rc =1;
                break;
            }
            // Obtain VMRELOCATE_elapsed
            token = strtok_r(NULL, blank, &buffer);
            if (token != NULL) {
                strcpy(vmRelocate_elapsed, token);
            } else {
                printf("ERROR: VMRELOCATE_elapsed is NULL\n");
                rc =1;
                break;
            }
            // Obtain VMRELOCATE_status
            token = strtok_r(NULL, "\0", &buffer);
            if (token != NULL) {
                strcpy(vmRelocate_status, token);
            } else {
                strcpy(vmRelocate_status, "");  // vmRelocate_status can be null so set it to empty string
            }
            printf("VMRELOCATE image: %s\n", vmRelocate_image);
            printf("VMRELOCATE source system: %s\n", vmRelocate_source_system);
            printf("VMRELOCATE destination system: %s\n", vmRelocate_destination_system);
            printf("VMRELOCATE by: %s\n", vmRelocate_by);
            printf("VMRELOCATE elapsed: %s\n", vmRelocate_elapsed);
            printf("VMRELOCATE status: %s\n", vmRelocate_status);
            printf("\n");
        }
    }
    return rc;
}
