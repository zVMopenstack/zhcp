/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliStatic.h"
#include "wrapperutils.h"

int staticImageChangesActivateDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    vmApiStaticImageChangesActivateDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Static_Image_Changes_Activate_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Static_Image_Changes_Activate_DM [-T] image_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Static_Image_Changes_Activate_DM to enable changes to the source\n"
                    "  directory to be made available to virtual images.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    printf("Enabling changes to %s's directory... ", image);
    rc = smStatic_Image_Changes_Activate_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            image, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Static_Image_Changes_Activate_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Static_Image_Changes_Activate_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int staticImageChangesDeactivateDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    vmApiStaticImageChangesDeactivateDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Static_Image_Changes_Deactivate_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Static_Image_Changes_Deactivate_DM [-T] image_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Static_Image_Changes_Deactivate_DM to prevent changes to the\n"
                    "  source directory from being made available to virtual images.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    printf("Preventing changes to %s's directory... ", image);
    rc = smStatic_Image_Changes_Deactivate_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            image, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Static_Image_Changes_Deactivate_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Static_Image_Changes_Deactivate_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int staticImageChangesImmediateDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    vmApiStaticImageChangesImmediateDmOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Static_Image_Changes_Immediate_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Static_Image_Changes_Immediate_DM [-T] image_name\n\n"
                    "DESCRIPTION\n"
                    "  Use Static_Image_Changes_Immediate_DM to make changes to the source\n"
                    "  directory immediately available to virtual images regardless of the\n"
                    "  current status of static image changes (active or inactive).\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    printf("Making changes to %s's directory immediately available... ", image);
    rc = smStatic_Image_Changes_Immediate_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password
            image, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Static_Image_Changes_Immediate_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Static_Image_Changes_Immediate_DM", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    return rc;
}
