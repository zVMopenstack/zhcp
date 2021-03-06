/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */

#include "smcliVMRM.h"
#include "wrapperutils.h"

int vmrmConfigurationQuery(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int i;
    int charNALength;
    char * charNABuffer;
    char * image = NULL;
    char * configFileName = NULL;
    char * configFileType = NULL;
    char * configDirName = NULL;
    vmApiVmrmConfigurationQueryOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:c:t:d:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'c':
                configFileName = optarg;
                break;

            case 't':
                configFileType = optarg;
                break;

            case 'd':
                configDirName = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  VMRM_Configuration_Query\n\n"
                    "SYNOPSIS\n"
                    "  smcli VMRM_Configuration_Query [-T] image_name [-c] config_file_name\n"
                    "  [-t] config_file_type [-d] config_dir_name\n\n"
                    "DESCRIPTION\n"
                    "  Use VMRM_Configuration_Query to query the contents of the VMRM configuration\n"
                    "  file.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n"
                    "    -c    The name of the configuration file\n"
                    "    -t    The file type of the configuration file\n"
                    "    -d    The fully-qualified Shared File System (SFS) directory name\n"
                    "          where the configuration file is located\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image || !configFileName || !configFileType || !configDirName) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    rc = smVMRM_Configuration_Query(vmapiContextP, "", 0, "",
            image, configFileName, configFileType, configDirName, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("VMRM_Configuration_Query", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("VMRM_Configuration_Query", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
    	charNABuffer = output->configurationFile;
        charNALength = output->configurationFileLength;

        // Print spaces through }, except for %, the rest print %xx values
        for (i = 0; i < charNALength; i++) {
            if ((charNABuffer[i] < 0x1F) ||
                    (charNABuffer[i] == 0x25) ||
                    (charNABuffer[i] > 0x7D)) {
                printf("%%%02.2x",charNABuffer[i]);
            } else {
                printf("%c",charNABuffer[i]);
            }
        }
        printf("\n");
    }
    return rc;
}

int vmrmConfigurationUpdate(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    int syncheck = -1;
    char * image = NULL;
    char * configFileName = NULL;
    char * configFileType = NULL;
    char * configDirName = NULL;
    char * updateFile = NULL;
    vmApiVmrmConfigurationUpdateOutput* output;

    FILE * fp;
    int recordCount = 0;
    int recordWidth, maxRecordWidth;
    int c;
    char * ptr;


    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:c:t:d:s:u:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'c':
                configFileName = optarg;
                break;

            case 't':
                configFileType = optarg;
                break;

            case 'd':
                configDirName = optarg;
                break;

            case 's':
                syncheck = atoi(optarg);
                break;

            case 'u':
                updateFile = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  VMRM_Configuration_Update\n\n"
                    "SYNOPSIS\n"
                    "  smcli VMRM_Configuration_Update [-T] image_name [-c] config_file_name\n"
                    "  [-t] config_file_type [-d] config_dir_name\n\n"
                    "DESCRIPTION\n"
                    "  Use VMRM_Configuration_Update to add, delete, and change VMRM configuration\n"
                    "  file statements.\n\n"
                    "  The following options are required:\n"
                    "    -T    This must match an entry in the authorization file\n"
                    "    -c    The name of the configuration file\n"
                    "    -t    The file type of the configuration file\n"
                    "    -d    The fully-qualified Shared File System (SFS) directory name\n"
                    "          where the configuration file is located\n"
                    "    -s    Specify a 1 to choose the SYNCHECK option, meaning that only a syntax\n"
                    "          check of the configuration is done, without processing a configuration\n"
                    "          file update. Otherwise, specify a 0 to indicate that both a syntax\n"
                    "          check and a configuration file update should occur.\n"
                    "    -u    A new, complete VMRM configuration file to syntax-check or to replace\n"
                    "          the old file.\n");
                return 1;
                break;

            default:
                return 1;
                break;
        }

    if (!image || !configFileName || !configFileType || !configDirName || (syncheck < 0) || !updateFile) {
        printf("ERROR: Missing required options\n");
        return 1;
    }

    // Open the user entry file
    fp = fopen(updateFile, "r");
    if (NULL == fp) {
       printf("\nERROR: Failed to open file %s\n", updateFile);
       return 2;
    }

    // Count the number of lines and width of widest line
    recordWidth = 0;
    maxRecordWidth = 0;
    while ((c = fgetc(fp)) != EOF) {
        ++recordWidth;
        if (c == '\n') {
            recordCount++;
            if (recordWidth > maxRecordWidth)
               maxRecordWidth = recordWidth;
            recordWidth = 0;
        }
    }

    // Reset position to start of file
    rewind(fp);

    // Create update record array 
    vmApiUpdateRecord *record;
    if (recordCount > 0) {
        if (0 == (record = malloc(recordCount * sizeof(vmApiUpdateRecord))))
            return MEMORY_ERROR;
    }

    // Create input buffer
    char *line[recordCount+1];
    if (maxRecordWidth > 0) {
        for (c = 0; c < recordCount+1; c++) {
           if (0 == (line[c] = malloc(maxRecordWidth+1))) {
              int i;
              for (i = 0; i < c; c++) 
                 free(line[i]);
              free(record);
              return MEMORY_ERROR;
           }
        }
    }

    // Read in user entry from file
    c = 0;
    while (fgets(line[c], maxRecordWidth+1, fp) != NULL) {
       // Replace newline with null terminator
       ptr = strstr(line[c], "\n");
       if (ptr != NULL) 
          strncpy(ptr, "\0", 1);
       // If blank line, leave one blank charascter
       if (strlen(line[c]) == 0)
          strncpy(line[c]," \0",2);
       record[c].updateRecordLength = strlen(line[c])+1;
       record[c].updateRecord = line[c];
       c++;
    }

    // Close file
    fclose(fp);

    printf("Updating VMRM configuration... ");
    rc = smVMRM_Configuration_Update(vmapiContextP, "", 0, "", image, configFileName, configFileType, configDirName,
            syncheck, recordCount, (vmApiUpdateRecord *) record, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("VMRM_Configuration_Update", rc, vmapiContextP);
    } else {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("VMRM_Configuration_Update", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    }
    // Print syntax error log if there is any
    if (output->logRecordCount > 0) {
        printf("Error log:\n");
        for(c = 0; c < output->logRecordCount; c++) {
           printf("  %.*s\n", output->logRecordInfoList[c].logRecordLength ,output->logRecordInfoList[c].logRecord);
        }
    }

    for(c = 0; c < recordCount; c++) 
       free(line[c]);
    free(record);

    return rc;
}

int vmrmMeasurementQuery(int argC, char* argV[], struct _vmApiInternalContext* vmapiContextP) {
    int rc;
    int option;
    char * image = NULL;
    vmApiVmrmMeasurementQueryOutput* output;

    // Options that have arguments are followed by a : character
    while ((option = getopt(argC, argV, "T:h?")) != -1)
        switch (option) {
            case 'T':
                image = optarg;
                break;

            case 'h':
                printf("NAME\n"
                    "  VMRM_Measurement_Query\n\n"
                    "SYNOPSIS\n"
                    "  smcli VMRM_Measurement_Query [-T] image_name\n\n"
                    "DESCRIPTION\n"
                    "  Use VMRM_Measurement_Query to obtain current VMRM measurement values.\n\n"
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

    rc = smVMRM_Measurement_Query(vmapiContextP, "", 0, "", image, &output);

    if (rc) {
        printAndLogSmapiCallReturnCode("VMRM_Measurement_Query", rc, vmapiContextP);
    } else if (output->common.returnCode || output->common.reasonCode) {
        // Handle SMAPI return code and reason code
        rc = printAndLogSmapiReturnCodeReasonCodeDescription("VMRM_Measurement_Query", output->common.returnCode,
                output->common.reasonCode, vmapiContextP);
    } else {
        printf("File name: %s\n"
               "File time stamp: %s\n"
               "Query time stamp: %s\n", output->fileName, output->fileTimestamp, output->queryTimestamp);

        int i;
        printf("Workload:\n");
        for (i = 0; i < output->workloadCount; i++) {
            printf("  %s\n", output->workloadInfoList[i].workloadRecord);
        }
    }
    return rc;
}
