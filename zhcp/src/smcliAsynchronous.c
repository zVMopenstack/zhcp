/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smcliAsynchronous.h"
#include "wrapperutils.h"

int asynchronousNotificationDisableDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int encoding = -1;
    int entity = -1;
    int communication = -1;
    int portNumber = -1;
    char* targetIdentifier = NULL;
    char* ip = NULL;
    char* subscriberData = "";
    vmApiAsynchronousNotificationDisableDmOutput* output;

    // Parse the command-line arguments
    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:t:c:p:i:e:s:h?")) != -1)
        switch (option) {
            case 'T':
            	targetIdentifier = optarg;
                break;

            case 't':
                entity = atoi(optarg);
                break;

            case 'c':
                communication = atoi(optarg);
                break;

            case 'p':
                portNumber = atoi(optarg);
                break;

            case 'i':
                ip = optarg;
                break;

            case 'e':
                encoding = atoi(optarg);
                break;

            case 's':
                subscriberData = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Asynchronous_Notification_Disable_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Asynchronous_Notification_Disable_DM [-T] target_identifier\n"
                    "    [-t] entity_type [-c] communication_type [-p] port_number\n"
                    "    [-i] ip_address [-e] encoding [-s] subscriber_data\n\n"
                    "DESCRIPTION\n"
                    "  Use Asynchronous_Notification_Disable_DM to end notification of updates to\n"
                    "  specified entities as they occur.\n\n"
                    "  The following options are required:\n"
                    "    -T    The userid for which notifications will be disabled\n"
                    "    -t    The entity type for which notifications will be sent:\n"
                    "            1: DIRECTORY\n"
                    "    -c    The communication type used for notifications:\n"
                    "            1: TCP\n"
                    "            2: UDP\n"
                    "    -p    The port number of the socket that will no longer be receiving the\n"
                    "          notifications\n"
                    "    -i    The IPV4 dotted-decimal IP address of the socket that will no longer\n"
                    "          receive the notifications\n"
                    "    -e    The encoding of the notification data string:\n"
                    "            0: Unspecified\n"
                    "            1: ASCII\n"
                    "            2: EBCDIC\n"
                    "    -s    The matching subscriber data\n");
                return 1;
                break;

            default:
            	return 1;
                break;
        }

    if (!targetIdentifier || (entity < 0) || (communication < 0) || (portNumber < 0) || !ip || (encoding < 0) ) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    if (encoding == 2) {
    	if (strlen(subscriberData) > 0) {
    		int subscriberDataLength = strlen(subscriberData);
    	    char ebcdicString[subscriberDataLength];
            convertASCIItoEBCDIC(vmapiContextP, subscriberData, ebcdicString);
            memmove(subscriberData, ebcdicString, subscriberDataLength);
    	}
    }
    printf("Ending notification of updates to %s... ", targetIdentifier);
    rc = smAsynchronous_Notification_Disable_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
    		targetIdentifier, entity, communication, portNumber, ip, encoding, strlen(subscriberData), subscriberData, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Asynchronous_Notification_Disable_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Asynchronous_Notification_Disable_DM",
                output->common.returnCode, output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int asynchronousNotificationEnableDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int entity = -1;
    int subscription = -1;
    int communication = -1;
    int portNumber = -1;
    int encoding = -1;
    char* targetIdentifier = NULL;
    char* ip = NULL;
    char* subscriberData = "";
    vmApiAsynchronousNotificationEnableDmOutput* output;

    // Parse the command-line arguments
    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:t:r:c:p:i:e:s:h?")) != -1)
        switch (option) {
            case 'T':
            	targetIdentifier = optarg;
                break;

            case 't':
                entity = atoi(optarg);
                break;

            case 'r':
                subscription = atoi(optarg);
                break;

            case 'c':
                communication = atoi(optarg);
                break;

            case 'p':
                portNumber = atoi(optarg);
                break;

            case 'i':
                ip = optarg;
                break;

            case 'e':
                encoding = atoi(optarg);
                break;

            case 's':
                subscriberData = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Asynchronous_Notification_Enable_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Asynchronous_Notification_Enable_DM [-T] target_identifier\n"
                    "    [-t] entity_type [-r] subscription_type [-c] communication_type\n"
                    "    [-p] port_number [-i] ip_address [-e] encoding\n"
                    "    [-s] subscriber_data\n\n"
                    "DESCRIPTION\n"
                    "  Use Asynchronous_Notification_Enable_DM to begin notification of updates\n"
                    "  to a specified entity as the updates occur.\n\n"
                    "  The following options are required:\n"
                    "    -T    The image to be notified\n"
                    "    -t    The entity type for which notifications will be sent:\n"
                    "            1: DIRECTORY\n"
                    "    -r    The subscription type:\n"
                    "            1: The target_identifier will receive notifications for\n"
                    "               associated directory changes\n"
                    "            2: The target_identifier will not receive notifications\n"
                    "               for associated directory changes\n"
                    "    -c    The communication type used for notifications:\n"
                    "            1: TCP\n"
                    "            2: UDP\n"
                    "    -p    The port number of the socket that will receive the notifications\n"
                    "    -i    The IPV4 dotted-decimal IP address of the socket that will receive\n"
                    "          the notifications\n"
                    "    -e    The encoding of the notification data string:\n"
                    "            0: Unspecified\n"
                    "            1: ASCII\n"
                    "            2: EBCDIC\n"
                    "    -s    Anything the subscriber wishes to receive along with the notifications\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!targetIdentifier || (entity < 0) || (subscription < 0) || (communication < 0) || (portNumber < 0) || !ip || (encoding < 0)) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    printf("Beginning notification of updates to %s... ", targetIdentifier);
    rc = smAsynchronous_Notification_Enable_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
    		targetIdentifier, entity, subscription, communication, portNumber, ip, encoding, strlen(subscriberData), subscriberData, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Asynchronous_Notification_Enable_DM", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Asynchronous_Notification_Enable_DM",
                output->common.returnCode, output->common.reasonCode, vmapiContextP);
    }
    return rc;
}

int asynchronousNotificationQueryDM(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int i;
    int option;
    int encoding = -1;
    int entity = -1;
    int communication = -1;
    int portNumber = -1;
    char* targetIdentifier = NULL;
    char* ip = "";
    char* subscriberData = "";
    vmApiAsynchronousNotificationQueryDmOutput* output;

    // Parse the command-line arguments
    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:t:c:p:i:e:s:h?")) != -1)
        switch (option) {
            case 'T':
            	targetIdentifier = optarg;
                break;

            case 't':
                entity = atoi(optarg);
                break;

            case 'c':
                communication = atoi(optarg);
                break;

            case 'p':
                portNumber = atoi(optarg);
                break;

            case 'i':
                ip = optarg;
                break;

            case 'e':
                encoding = atoi(optarg);
                break;

            case 's':
                subscriberData = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  Asynchronous_Notification_Query_DM\n\n"
                    "SYNOPSIS\n"
                    "  smcli Asynchronous_Notification_Query_DM [-T] target_identifier\n"
                    "    [-t] entity_type  [-c] communication_type\n"
                    "    [-p] port_number [-i] ip_address [-e] encoding\n"
                    "    [-s] subscriber_data\n\n"
                    "DESCRIPTION\n"
                    "  Use Asynchronous_Notification_Query_DM to query which users\n"
                    "  are subscribed to receive notification of updates to specified\n"
                    "  entities.\n\n"
                    "  The following options are required:\n"
                    "    -T    The images to be notified\n"
                    "    -t    The entity type for which notifications will be sent:\n"
                    "            1: DIRECTORY\n"
                    "    -c    The communication type of the notification strings being queried:\n"
                    "            0: Unspecified\n"
                    "            1: TCP\n"
                    "            2: UDP\n"
                    "    -p    The port number of the socket that will receive the notifications\n"
                    "    -i    The IPV4 dotted-decimal IP address of the socket that will receive\n"
                    "          the notifications\n"
                    "    -e    The encoding of the notification strings being queried:\n"
                    "            0: Unspecified\n"
                    "            1: ASCII\n"
                    "            2: EBCDIC\n"
                    "    -s    Subscriber data\n"
                    "            Anything the subscriber wishes to receive along with the\n"
                    "            notifications\n"
                    "            '*': Selects all that qualify\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!targetIdentifier || (entity < 0) || (communication < 0)|| (portNumber < 0) || (encoding < 0) ) {
        printf("\nERROR: Missing required options\n");
        return 1;
    }

    rc = smAsynchronous_Notification_Query_DM(vmapiContextP, "", 0, "",  // Authorizing user, password length, password.
    		targetIdentifier, entity, communication, portNumber, ip, encoding, strlen(subscriberData), subscriberData, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("Asynchronous_Notification_Query_DM", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("Asynchronous_Notification_Query_DM",
                output->common.returnCode, output->common.reasonCode, vmapiContextP);
    } else {
    	for (i=0; i < output->notificationCount; i++) {
        printf("  User ID:            %s\n"
               "  Subscription Type:  %d\n"
               "  Communication Type: %d\n"
               "  Port Number:        %d\n"
               "  IP Address:         %s\n"
               "  Encoding:           %d\n"
               "  SubscriberData:     %s\n",
               output->notificationList[i].userid, output->notificationList[i].subscriptionType, output->notificationList[i].communicationType,
               output->notificationList[i].portNumber, output->notificationList[i].ipAddress, output->notificationList[i].encoding,
               output->notificationList[i].subscriberData);
    	}
    }

    return rc;
}
