/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliSSI.h"
#include "wrapperutils.h"

int ssiQuery(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int i = 0;
    char *token;
    char * buffer;  // Character pointer whose value is preserved between successive related calls to strtok_r
    char * blank = " ";
    char member_slot[1 + 1];
    char member_system_id[8 + 1];
    char member_state[9 + 1];
    char member_pdr_heartbeat[19 + 1];
    char member_received_heartbeat[19 + 1];
    vmApiSSIQueryOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "h?")) != -1)
        switch (option) {
            case 'h':
                printf("NAME\n"
                    "  SSI_Query\n\n"
                    "SYNOPSIS\n"
                    "  smcli SSI_Query\n\n"
                    "DESCRIPTION\n"
                    "  Use SSI_Query to obtain the SSI and system status.\n\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    rc = smSSI_Query(vmapiContextP, &output);


    if (rc) {
        printAndLogSmapiCallReturnCode("SSI_Query", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
    	rc = printAndLogSmapiReturnCodeReasonCodeDescription("SSI_Query", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        printf("ssi_name = %s\n", output->ssi_name);
        printf("ssi_mode = %s\n", output->ssi_mode);
        printf("ssi_pdr = %s\n", output->ssi_pdr);
        printf("cross_system_timeouts = %s\n", output->cross_system_timeouts);
        printf("output.ssiInfoCount = %d\n\n", output->ssiInfoCount);

        // Obtain the 5 members of the ssi_info_structure that are in the output->ssiInfo[i].vmapiString
        for (i = 0; i < output->ssiInfoCount; i++) {
            // Obtain member_slot
            token = strtok_r(output->ssiInfo[i].vmapiString, blank, &buffer);
            if (token != NULL) {
                strcpy(member_slot, token);
            } else {
                printf("ERROR: member_slot is NULL\n");
                rc =1;
                break;
            }

            // Obtain member_system_id
            token = strtok_r(NULL, blank, &buffer);
            if (token != NULL) {
                strcpy(member_system_id, token);
            } else {
                printf("ERROR: member_system_id is NULL\n");
                rc =1;
                break;
            }

            // Obtain member_state
            token = strtok_r(NULL, blank, &buffer);
            if (token != NULL) {
                strcpy(member_state, token);
            } else {
                printf("ERROR: member_state is NULL\n");
                rc =1;
                break;
            }

            //  Check member_state to see if it equals Available
            if (strcasecmp(member_state, "Available") == 0) {
                //  We are done there is no SSI data for this member_slot. This is not a error it just means
                //  this member_slot has no zVM in it. Continue to get the next member_slot if there is one.
                continue;
            } else if (strcasecmp(member_state, "Down") == 0) {
                //  There is no SSI data for this member_slot because this zVM is down
                printf("member_slot = %s\n", member_slot);
                printf("member_system_id = %s\n", member_system_id);
                printf("member_state = %s\n", member_state);
                printf("member_pdr_heartbeat = N/A\n");
                printf("member_received_heartbeat = N/A\n\n");
            }

            // Obtain member_pdr_heartbeat
            token = strtok_r(NULL, blank, &buffer);
            if (token != NULL) {
                strcpy(member_pdr_heartbeat, token);
            } else {
                printf("ERROR: member_pdr_heartbeat is NULL\n");
                rc =1;
                break;
            }

            // Obtain member_received_heartbeat
            token = strtok_r(NULL, "\0", &buffer);
            if (token != NULL) {
                strcpy(member_received_heartbeat, token);
            } else {
                printf("ERROR: member_received_heartbeat is NULL\n");
                rc =1;
                break;
            }

            printf("member_slot = %s\n", member_slot);
            printf("member_system_id = %s\n", member_system_id);
            printf("member_state = %s\n", member_state);
            printf("member_pdr_heartbeat = %s\n", member_pdr_heartbeat);
            printf("member_received_heartbeat = %s\n\n", member_received_heartbeat);
        }
    }

    return rc;
}

