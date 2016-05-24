/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "wrapperutils.h"

/**
 * @see wrapperutils.h: releaseContext
 */
void releaseContext(vmApiInternalContext* context) {
    smMemoryGroupFreeAll(context);
    smMemoryGroupTerminate(context);
}

/**
 * @see wrapperutils.h getContext
 */
void initializeContext(vmApiInternalContext* context, smMemoryGroupContext* memContext) {
    memset(context, 0, sizeof(context));
    memset(memContext, 0, sizeof(memContext));

    (context)->memContext = memContext;
    smMemoryGroupInitialize(context);
}

/**
 * Check if image name is between 1 and 8 characters
 */
int isImageNameInvalid(char* imageName) {
    if (strlen(imageName) < 1 || strlen(imageName) > 8) {
        printf("Error: User ID must be between 1 and 8 characters in length\n");
        return 1;
    } else {
        return 0;
    }
}

/**
 * Check if profile name is between 1 and 8 characters
 */
int isProfileNameInvalid(char* profileName) {
    if (strlen(profileName) < 1 || strlen(profileName) > 8) {
        printf("  User profile must be between 1 and 8 characters in length.\n"
            "Operation Failed\n");
        return 1;
    } else {
        return 0;
    }
}

/**
 * Check if device addresses are specified as 4-digit hexadecimal numbers
 */
int isDevNumberInvalid(char* devNumber) {
    if ((strlen(devNumber) == 4) && ((devNumber[0] > 47 && devNumber[0] < 58)
            || (devNumber[0] > 65 && devNumber[0] < 71) || (devNumber[0] > 97
            && devNumber[0] < 103))
            && ((devNumber[1] > 47 && devNumber[1] < 58) || (devNumber[1] > 65
                    && devNumber[1] < 71) || (devNumber[1] > 97 && devNumber[1]
                    < 103)) && ((devNumber[2] > 47 && devNumber[2] < 58)
            || (devNumber[2] > 65 && devNumber[2] < 71) || (devNumber[2] > 97
            && devNumber[2] < 103))
            && ((devNumber[3] > 47 && devNumber[3] < 58) || (devNumber[3] > 65
                    && devNumber[3] < 71) || (devNumber[3] > 97 && devNumber[3]
                    < 103))) {
        return 0;
    } else {
        printf("  Device addresses must be specified as 4-digit hexadecimal numbers.\n"
               "Operation Failed\n");
        return 1;
    }
}

/**
 * Trim a specified string
 */
void trim(char * s) {
    // Length of specified string
    int len = strlen(s);
    int end = len - 1;
    int start = 0;
    int i = 0;

    // Find non-blank space from left
    while ((start < len) && (s[start] <= ' ')) {
        start++;
    }

    // Find non-blank space from right
    while ((start < end) && (s[end] <= ' ')) {
        end--;
    }

    if (start > end) {
        memset(s, '\0', len);
        return;
    }

    for (i = 0; (i + start) <= end; i++) {
        s[i] = s[start + i];
    }

    // Trim string
    memset((s + i), '\0', len - i);
}


static unsigned char a2e[256] = {
          0,   1,   2,   3,  55,  45,  46,  47,  22,   5,  37,  11,  12,  13,  14,  15,
         16,  17,  18,  19,  60,  61,  50,  38,  24,  25,  63,  39,  28,  29,  30,  31,
         64,  79, 127, 123,  91, 108,  80, 125,  77,  93,  92,  78, 107,  96,  75,  97,
        240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 122,  94,  76, 126, 110, 111,
        124, 193, 194, 195, 196, 197, 198, 199, 200, 201, 209, 210, 211, 212, 213, 214,
        215, 216, 217, 226, 227, 228, 229, 230, 231, 232, 233,  74, 224,  90,  95, 109,
        121, 129, 130, 131, 132, 133, 134, 135, 136, 137, 145, 146, 147, 148, 149, 150,
        151, 152, 153, 162, 163, 164, 165, 166, 167, 168, 169, 192, 106, 208, 161,   7,
         32,  33,  34,  35,  36,  21,   6,  23,  40,  41,  42,  43,  44,   9,  10,  27,
         48,  49,  26,  51,  52,  53,  54,   8,  56,  57,  58,  59,   4,  20,  62, 225,
         65,  66,  67,  68,  69,  70,  71,  72,  73,  81,  82,  83,  84,  85,  86,  87,
         88,  89,  98,  99, 100, 101, 102, 103, 104, 105, 112, 113, 114, 115, 116, 117,
        118, 119, 120, 128, 138, 139, 140, 141, 142, 143, 144, 154, 155, 156, 157, 158,
        159, 160, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
        184, 185, 186, 187, 188, 189, 190, 191, 202, 203, 204, 205, 206, 207, 218, 219,
        220, 221, 222, 223, 234, 235, 236, 237, 238, 239, 250, 251, 252, 253, 254, 255
};

static unsigned char e2a[256] = {
          0,   1,   2,   3, 156,   9, 134, 127, 151, 141, 142,  11,  12,  13,  14,  15,
         16,  17,  18,  19, 157, 133,   8, 135,  24,  25, 146, 143,  28,  29,  30,  31,
        128, 129, 130, 131, 132,  10,  23,  27, 136, 137, 138, 139, 140,   5,   6,   7,
        144, 145,  22, 147, 148, 149, 150,   4, 152, 153, 154, 155,  20,  21, 158,  26,
         32, 160, 161, 162, 163, 164, 165, 166, 167, 168,  91,  46,  60,  40,  43,  33,
         38, 169, 170, 171, 172, 173, 174, 175, 176, 177,  93,  36,  42,  41,  59,  94,
         45,  47, 178, 179, 180, 181, 182, 183, 184, 185, 124,  44,  37,  95,  62,  63,
        186, 187, 188, 189, 190, 191, 192, 193, 194,  96,  58,  35,  64,  39,  61,  34,
        195,  97,  98,  99, 100, 101, 102, 103, 104, 105, 196, 197, 198, 199, 200, 201,
        202, 106, 107, 108, 109, 110, 111, 112, 113, 114, 203, 204, 205, 206, 207, 208,
        209, 126, 115, 116, 117, 118, 119, 120, 121, 122, 210, 211, 212, 213, 214, 215,
        216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
        123,  65,  66,  67,  68,  69,  70,  71,  72,  73, 232, 233, 234, 235, 236, 237,
        125,  74,  75,  76,  77,  78,  79,  80,  81,  82, 238, 239, 240, 241, 242, 243,
         92, 159,  83,  84,  85,  86,  87,  88,  89,  90, 244, 245, 246, 247, 248, 249,
         48,  49,  50,  51,  52,  53,  54,  55,  56,  57, 250, 251, 252, 253, 254, 255
};

unsigned char ASCIItoEBCDIC(const unsigned char c) {
        return a2e[c];
}

unsigned char EBCDICtoASCII(const unsigned char c) {
        return e2a[c];
}

/**
 * Convert the passed in string from EBCDIC to ASCII
 */
void convertEBCDICtoASCII(struct _vmApiInternalContext *vmapiContextP, char *ebcdicString, char *asciiString) {
    int rc = 0;
    int i;
    int sizeOfebcdicString;

    sizeOfebcdicString = strlen(ebcdicString);

    for (i = 0; i < sizeOfebcdicString; i++) {
        asciiString[i] = EBCDICtoASCII(ebcdicString[i]);
    }
}

/**
 * Convert the passed in string from ASCII to EBCDIC
 */
void convertASCIItoEBCDIC(struct _vmApiInternalContext *vmapiContextP, char *asciiString, char *ebcdicString) {
    int rc = 0;
    int i;
    int sizeOfasciiString = strlen(asciiString);
    for (i = 0; i < sizeOfasciiString; i++) {
        ebcdicString[i] = ASCIItoEBCDIC(asciiString[i]);
    }
}


/**
 * Print and log return code from call to SMAPI error.
 */
void printAndLogSmapiCallReturnCode(char * class, int rc, struct _vmApiInternalContext *vmapiContextP) {
    char line[256];
    char errMsg[128];

    if (rc == -2) {
        sprintf(errMsg, "Invalid data\n");
    } else if(rc == -3) {
        sprintf(errMsg, "Processing error\n");
    } else if(rc == -100) {
        sprintf(errMsg, "Socket obtain error\n");
    } else if(rc == -101) {
        sprintf(errMsg, "Socket connect refused error\n");
    } else if(rc == -102) {
        sprintf(errMsg, "Socket connect try again error\n");
    } else if(rc == -103) {
        sprintf(errMsg, "Socket timeout error\n");
    } else if(rc == -104) {
        sprintf(errMsg, "Socket read error\n");
    } else if(rc == -105) {
        sprintf(errMsg, "Socket read retry error\n");
    } else if(rc == -106) {
        sprintf(errMsg, "Socket write error\n");
    } else if(rc == -107) {
        sprintf(errMsg, "Socket write retry error\n");
    } else if(rc == -108) {
        sprintf(errMsg, "Socket processing error\n");
    } else if(rc == -999) {
        sprintf(errMsg, "Memory error\n");
    } else if(rc == -4002) {
        sprintf(errMsg, "Parser error invalid table\n");
    } else if(rc == -4003) {
        sprintf(errMsg, "Parser error invalid string size\n");
    } else {
        sprintf(errMsg, "Unknown\n");
    }

    printf("Failed\n"
            "  Return Code: %d\n"
            "  Description: %s\n", rc, errMsg);
    // If the trace file has not been read yet, do it.
    if (!(vmapiContextP->smTraceDetails->traceFileRead)) {
        readTraceFile(vmapiContextP);
    }

    TRACE_START(vmapiContextP, TRACEAREA_SMCLI, TRACELEVEL_ERROR);
        sprintf(line, "Call to SMAPI error. SMAPI API: %s RC = %d Description: %s\n", class, rc, errMsg);
    TRACE_END_DEBUG(vmapiContextP, line);
}

/**
 * Print and log SMAPI return code and reason code and description
 */
int printAndLogSmapiReturnCodeReasonCodeDescription(char * class, int returnCode, int reasonCode,
         struct _vmApiInternalContext *vmapiContextP) {
	int rc = 1;  // Set the return rc to a fail. If the SMAPI rc and rs are good then rc will be set to 0
    // Handle return code and reason code
    if (returnCode || reasonCode) {
        if (returnCode == 592) {
        	printf("Started\n");
        } else {
        	printf("Failed\n");
        }
        printf("  Return Code: %d\n"
               "  Reason Code: %d\n", returnCode, reasonCode);

        // Print description if one exists
        if (returnCode > -1 && reasonCode > -1) {
            printSmapiDescriptionAndLogError(class, returnCode, reasonCode, vmapiContextP);
        }
    } else {
        printf("Done\n");
        rc = 0;
    }
    return rc;
}

/**
 * Print and log SMAPI return code and reason code and description and Error Buffer
 * if it exist
 */
int printAndLogSmapiReturnCodeReasonCodeDescriptionAndErrorBuffer(char * class, int rc, int returnCode, int reasonCode,
         int errorLength, char * errorBuffer, struct _vmApiInternalContext *vmapiContextP) {
    int i;
    int j;
    int temp;
    int smcliRC;  // printAndLogSmapiReturnCodeReasonCodeDescription() will set this
    char * stringStart;
    char line[errorLength +1024];  // Not sure how many thing we get make real big to be safe
    // If the trace file has not been read yet, do it.
    if (!(vmapiContextP->smTraceDetails->traceFileRead)) {
        readTraceFile(vmapiContextP);
    }

    // Handle return code and reason code
    smcliRC = printAndLogSmapiReturnCodeReasonCodeDescription(class, returnCode, reasonCode, vmapiContextP);

    // In case the error buffer has null strings (must have rc=0), print each one
    if (!rc) {
        if (errorLength > 0) {
            printf("Details: ");
            strcpy(line, "SMAPI ERROR BUFFER details:\n");
            stringStart = errorBuffer;
            for (i = 0; i < errorLength; i += temp) {
                printf("%s\n", stringStart);
                strcat(line, stringStart);
                strcat(line, "\0");
                strcat(line, "\n");
                temp = strlen(errorBuffer) + 1;
                stringStart += temp;
            }
            TRACE_START(vmapiContextP, TRACEAREA_SMCLI, TRACELEVEL_ERROR);
            TRACE_END_DEBUG(vmapiContextP, line);
        }
    }
    return smcliRC;
}
