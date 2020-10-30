#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BOARD.h"
#include "Message.h"
#include "Oled.h"
#include "OledDriver.h"
#include "xc.h"

/* ENUM AND STRUCT FOR MESSAGEDECODE */
typedef enum {
    WAITING_FOR_START,
    RECORD_PAYLOAD,
    RECORD_CHECKSUM
} DecodeState;

typedef struct {
    char payloadDecode[MESSAGE_MAX_LEN];
    char checksumDecode[MESSAGE_MAX_LEN];
    int payloadPointer;
    int checksumPointer;
    DecodeState decodeState;
} Decoder;

static Decoder decodeSM = {
    {},
    {}, WAITING_FOR_START
};

uint8_t Message_CalculateChecksum(const char* payload)
{
    uint8_t checksum = 0x00;
    int i = 0;
    while (payload[i] != NULL) {
        checksum ^= payload[i];
        i++;
    }
    return checksum;
}

int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event)
{
    char newChecksumString[MESSAGE_CHECKSUM_LEN];
    char* param0String;
    char* param1String;
    char* param2String;
    char* typeString;
    char payloadString[MESSAGE_MAX_PAYLOAD_LEN];
    strcpy(payloadString, payload);

    /* CHECKSUM COMPARISON CHECK */
    uint8_t newChecksum = Message_CalculateChecksum(payload);
    sprintf(newChecksumString, "%X\n", newChecksum);
    if (strcmp(newChecksumString, checksum_string) != 0) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    
    /* SPLITS STRING AND STORES INTO BB_EVENT */
    } else {
        typeString = strtok(payloadString, ","); // char * or char[]?

        param0String = strtok(NULL, ",");
        if (strcmp(typeString, "CHA") == 0) {
            message_event->type = BB_EVENT_CHA_RECEIVED;
            message_event->param0 = atoi(param0String);
        } else if (strcmp(typeString, "ACC") == 0) {
            message_event->type = BB_EVENT_ACC_RECEIVED;
            message_event->param0 = atoi(param0String);
        } else if (strcmp(typeString, "REV") == 0) {
            message_event->type = BB_EVENT_REV_RECEIVED;
            message_event->param0 = atoi(param0String);
        } else if (strcmp(typeString, "SHO") == 0) {
            message_event->type = BB_EVENT_SHO_RECEIVED;
            param1String = strtok(NULL, ",");
            message_event->param0 = atoi(param0String);
            message_event->param1 = atoi(param1String);
        } else if (strcmp(typeString, "RES") == 0) {
            message_event->type = BB_EVENT_RES_RECEIVED;
            param1String = strtok(NULL, ",");
            param2String = strtok(NULL, ",");
            message_event->param0 = atoi(param0String);
            message_event->param1 = atoi(param1String);
            message_event->param2 = atoi(param2String);
        } else {
            message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }
        return SUCCESS;
    }
}

int Message_Encode(char *message_string, Message message_to_encode)
{
    char messageFull[MESSAGE_MAX_LEN];
    char payload[MESSAGE_MAX_LEN];
    char payloadString[MESSAGE_MAX_LEN];
    uint8_t messageChecksum;
    int length = 0;

    /* ENCODES BASED ON MESSAGE TYPE, STORES INTO A STRING */
    if (message_to_encode.type == MESSAGE_NONE) {
        return length;
    } else if (message_to_encode.type == MESSAGE_CHA) {
        sprintf(payload, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
        strcpy(payloadString, payload);
    } else if (message_to_encode.type == MESSAGE_ACC) {
        sprintf(payload, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
        strcpy(payloadString, payload);
    } else if (message_to_encode.type == MESSAGE_REV) {
        sprintf(payload, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
        strcpy(payloadString, payload);
    } else if (message_to_encode.type == MESSAGE_SHO) {
        sprintf(payload, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
        strcpy(payloadString, payload);
    } else if (message_to_encode.type == MESSAGE_RES) {
        sprintf(payload, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
        strcpy(payloadString, payload);
    } else if (message_to_encode.type == MESSAGE_NONE){
        return 0; 
    }


    messageChecksum = Message_CalculateChecksum(payloadString);

    /* PAYLOAD AND CHECKSUM STRING COMBINED AND RETURNED */
    sprintf(messageFull, MESSAGE_TEMPLATE, payloadString, messageChecksum);
    strcpy(message_string, messageFull);

    length = strlen(message_string);
    return length;
}

int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event)
{

    switch (decodeSM.decodeState) {

        /* WAITS FOR START OF MESSAGE WITH $ INPUT */
    case WAITING_FOR_START:
        decodeSM.payloadPointer = 0;
        decodeSM.checksumPointer = 0;
        if (char_in == '$') {
            decodeSM.decodeState = RECORD_PAYLOAD;
            decoded_message_event->type = BB_EVENT_NO_EVENT;
            return SUCCESS;
        } else {
            decoded_message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }
        break;

        /* DECODES PAYLOAD, CHECKS FOR ERRORS, AND WAITS FOR * TO TRANSITION */
    case RECORD_PAYLOAD:
        if (char_in == '$' || char_in == '\n') {
            decodeSM.decodeState = WAITING_FOR_START;
            decoded_message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        } else if (strlen(decodeSM.payloadDecode) > MESSAGE_MAX_PAYLOAD_LEN) {
            decodeSM.decodeState = WAITING_FOR_START;
            decoded_message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        } else if (char_in == '*') {
            decodeSM.decodeState = RECORD_CHECKSUM;
            return SUCCESS;
        } else {
            decodeSM.payloadDecode[decodeSM.payloadPointer] = char_in;
            decodeSM.payloadPointer++;
            return SUCCESS;
        }
        break;

        /* RECORDS CHECKSUM, PARSES IF NO ERRORS AND REACHED END OF STRING */
    case RECORD_CHECKSUM:
        if (strlen(decodeSM.checksumDecode) > MESSAGE_CHECKSUM_LEN) {
            decodeSM.decodeState = WAITING_FOR_START;
            decoded_message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        } else if ((char_in <= '9' && char_in >= '0') || (char_in == 'A') || (char_in == 'B') || (char_in == 'C') || (char_in == 'D') || (char_in == 'E') || (char_in == 'F')) {
            decodeSM.checksumDecode[decodeSM.checksumPointer] = char_in;
            decodeSM.checksumPointer++;
            return SUCCESS;
        } else if (char_in == '\n') {
            decodeSM.checksumDecode[decodeSM.checksumPointer] = char_in;
            decodeSM.decodeState = WAITING_FOR_START;
            Message_ParseMessage(decodeSM.payloadDecode, decodeSM.checksumDecode, decoded_message_event);

            /* RESETS CHECKSUM AND PAYLOAD STRINGS */
            memset(decodeSM.checksumDecode, '\0', strlen(decodeSM.checksumDecode));
            memset(decodeSM.payloadDecode, '\0', strlen(decodeSM.payloadDecode));

            if (decoded_message_event->type == BB_EVENT_ERROR) {
                return STANDARD_ERROR;
            } else {
                return SUCCESS;
            }
        } else {
            decodeSM.decodeState = WAITING_FOR_START;
            decoded_message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }
        break;
        
    }
    
    return STANDARD_ERROR;
}