/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliPage.h"
#include "wrapperutils.h"

int pageorSpoolVolumeAdd(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int entryCount = 0;
    int option;
    char * targetIdentifier = NULL;
    char * entryArray[8];
    vmApiPageOrSpoolVolumeAddOutput * output;

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
                if (entryCount < 8) {
                    entryArray[entryCount] = optarg;
                    entryCount++;
                } else {
                    printf("ERROR: Too many -k values entered\n");
                    return INVALID_DATA;
                }
                break;

            case 'h':
                printf("NAME\n"
                    "  Page_or_Spool_Volume_Add\n\n"
                    "SYNOPSIS\n"
                    "  smcli Page_or_Spool_Volume_Add [-T] targetIdentifier\n"
                    "    [-k] 'entry1' [-k] 'entry2' ...\n\n"
                    "DESCRIPTION\n"
                    "  Use Page_or_Spool_Volume_Add to add a full volume page or spool disk to the\n"
                    "  system.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file.\n"
                    "    -k    A keyword=value item.\n"
                    "          They may be specified in any order. They must be inside\n"
                    "          single quotes. For example -k 'key=value'. Possible keywords are:\n"
                    "              vol_addr -  The real address of the volume to be used for page or\n"
                    "                          spool space. This is a required parameter\n"
                    "              volume_label - The name to be associated with the newly formatted\n"
                    "                             volume. This is a required parameter\n"
                    "              volume_use - One of the following:\n"
                    "                PAGE - The volume is to be formatted and used as a page volume.\n"
                    "                SPOOL -The volume is to be formatted and used as a spool volume\n"
                    "                This is a required parameter\n"
                    "              system_config_name - The file name of the system configuration\n"
                    "                                   file. DMSSICNF COPY file contains the default\n"
                    "              system_config_type - The file type of the system configuration\n"
                    "                                   file. DMSSICNF COPY file contains the default\n"
                    "              parm_disk_owner - The owner of the parm disk. DMSSICNF COPY file\n"
                    "                                contains the default\n"
                    "              parm_disk_number - Number of the parm disk, as defined in the\n"
                    "                                 VSMWORK1 directory. DMSSICNF COPY file contains\n"
                    "                                 the default\n"
                    "              parm_disk_password - The multiwrite password for the parm disk\n"
                    "                                   DMSSICNF COPY file contains the default\n"
                    "                Note: The character ',' is used to indicate no password. Therefore\n"
                    "                ',' cannot be the password.\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!targetIdentifier ||  entryCount < 3)  {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Adding a full volume page or spool disk to the system... ");
    rc = smPage_or_Spool_Volume_Add(vmapiContextP, "", 0, "",  targetIdentifier, entryCount, entryArray, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Page_or_Spool_Volume_Add", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Page_or_Spool_Volume_Add", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}
