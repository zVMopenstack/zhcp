/**
 * IBM (C) Copyright 2013 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smSocket.h"
#include "vmapiUnDocumented.h"
#include "smapiTableParser.h"
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define PARSER_TABLE_NAME     Image_IPL_Device_Query_Layout
#define OUTPUT_STRUCTURE_NAME vmApiImageIplDeviceQueryOutput

/**
 * Image_IPL_Device_Query SMAPI interface
 */
int smImage_IPL_Device_Query(struct _vmApiInternalContext* vmapiContextP,
        char * userid, vmApiImageIplDeviceQueryOutput ** outData) {
    const char * const functionName = "Image_IPL_Device_Query";
    int tempSize;
    char * cursor;
    char * stringCursor;  // Used for outData string area pointer
    int arrayCount;
    int totalStringSize;
    int rc;
    int sockDesc;
    int requestId;

    int inputSize = 4 + 1 + strlen(functionName) + 1 /* Null term */+ strlen(
            userid) + 1 /* Null term */;
    char * inputP = 0;
    char * smapiOutputP = 0;
    char line[LINESIZE];
    int i;

    // Build SMAPI input parameter buffer
    if (0 == (inputP = malloc(inputSize)))
        return MEMORY_ERROR;
    cursor = inputP;
    PUT_INT(inputSize - 4, cursor);

    *cursor = 0xFF;  // Separator
    cursor++;

    tempSize = strlen(functionName);
    strcpy(cursor, functionName);
    cursor += tempSize + 1;

    tempSize = strlen(userid);
    strcpy(cursor, userid);
    cursor += tempSize + 1;

    // This routine will send SMAPI the input, delete the input storage
    // and call the table parser to set the output in outData
    rc = getAndParseSmapiBuffer(vmapiContextP, &inputP, inputSize,
            PARSER_TABLE_NAME,  // Integer table
            TO_STRING(PARSER_TABLE_NAME),  // String name of the table
            (char * *) outData);
    return rc;
}
