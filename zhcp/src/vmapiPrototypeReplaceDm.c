/**
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "smSocket.h"
#include "vmapiPrototype.h"
#include "smapiTableParser.h"
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define PARSER_TABLE_NAME     Prototype_Replace_DM_Layout
#define OUTPUT_STRUCTURE_NAME vmApiPrototypeReplaceDmOutput

/**
 * Prototype_Replace_DM SMAPI interface
 */
int smPrototype_Replace_DM(struct _vmApiInternalContext* vmapiContextP,
        char * userid, int passwordLength, char * password,
        char * targetIdentifier, int recordArrayCount,
        vmApiPrototypeRecordList * recordArrayData,
        vmApiPrototypeReplaceDmOutput ** outData) {
    const char * const functionName = "Prototype_Replace_DM";
    tableParserParms parserParms;
    int tempSize;
    char * cursor;
    char * stringCursor;  // Used for outData string area pointer
    int arrayCount;
    int totalStringSize;
    int rc;
    int sockDesc;
    int requestId;

    int inputSize = 4 + 4 + strlen(functionName) + 4 + strlen(userid) + 4
            + passwordLength + 4 + strlen(targetIdentifier) + 4 /*record array length */;
    char * inputP = 0;
    char * smapiOutputP = 0;
    char line[LINESIZE];
    int i;
    int recordTotal = 0;

    for (i = 0; i < recordArrayCount; i++) {
        inputSize += 4;  // Record length integer
        inputSize += recordArrayData[i].recordNameLength;  // Data length

        recordTotal += 4;
        recordTotal += recordArrayData[i].recordNameLength;
    }

    // Build SMAPI input parameter buffer
    if (0 == (inputP = malloc(inputSize + 4)))
        return MEMORY_ERROR;
    cursor = inputP;
    PUT_INT(inputSize - 4, cursor);

    tempSize = strlen(functionName);
    PUT_INT(tempSize, cursor);
    memcpy(cursor, functionName, tempSize);
    cursor += tempSize;

    tempSize = strlen(userid);  // Userid 1..8 or 0..8 chars
    PUT_INT(tempSize, cursor);
    if (tempSize > 0) {
        memcpy(cursor, userid, tempSize);
        cursor += tempSize;
    }

    tempSize = passwordLength;  // Password 1..200 or 0..200 chars
    PUT_INT(tempSize, cursor);
    if (tempSize > 0) {
        memcpy(cursor, password, tempSize);
        cursor += tempSize;
    }

    tempSize = strlen(targetIdentifier);  // Target identifier 1..8 (image name)
    PUT_INT(tempSize, cursor);
    memcpy(cursor, targetIdentifier, tempSize);
    cursor += tempSize;

    // Add in the length of record array then the data if available
    PUT_INT(recordTotal, cursor);
    if (recordTotal > 0) {
        for (i = 0; i < recordArrayCount; i++) {
            tempSize = recordArrayData[i].recordNameLength;  // Data length
            PUT_INT(tempSize, cursor);

            memcpy(cursor, recordArrayData[i].recordName, tempSize);
            cursor += tempSize;
        }
    }

    // This routine will send SMAPI the input, delete the input storage
    // and call the table parser to set the output in outData
    rc = getAndParseSmapiBuffer(vmapiContextP, &inputP, inputSize,
            PARSER_TABLE_NAME,  // Integer table
            TO_STRING(PARSER_TABLE_NAME),  // String name of the table
            (char * *) outData);
    return rc;
}
