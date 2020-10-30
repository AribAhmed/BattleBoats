#include "Message.h"
#include "BOARD.H"

#include <xc.h>


#include <math.h>
#include "xc.h"
#include <string.h>
#include "Oled.h"
#include "OledDriver.h"
#include "Battleboats.h"
#include <stdio.h> 
#include <stdlib.h> 

int main()
{
    BOARD_Init();


    printf("\nWelcome to Message Test, compiled on %s %s \n", __DATE__, __TIME__);


    /* TEST FOR MESSAGE_CALCULATECHECKSUM */
    
    const char payload[] = "test1";
    char newChecksumTestString[MESSAGE_CHECKSUM_LEN];
    uint8_t checksumTest = Message_CalculateChecksum(payload);
    sprintf(newChecksumTestString, "%X", checksumTest);
    if (strcmp(newChecksumTestString, "27") == 0) {
        printf("\nPASSED: Checksum Test");
    } else {
        printf("\nchecksum test string: %c%c%c", newChecksumTestString[0], newChecksumTestString[1], newChecksumTestString[2]);
        printf("\nFAILED: Checksum Test");
    }

    /* TEST FOR MESSAGE_ENCODE */
    
    char message_output[MESSAGE_MAX_LEN];
    static Message encodeTest = {MESSAGE_RES, 4, 8, 1};
    Message_Encode(message_output, encodeTest);
    if (strcmp(message_output, "$RES,4,8,1*55\n") == 0) {
        printf("\nPASSED: Message Encode Test 1/2\n");
    } else {
        printf("\nEncoded Message: %c%c%c%c%c%c%c%c%c%c%c%c%c%c", message_output[0], message_output[1],
                message_output[2], message_output[3], message_output[4], message_output[5],
                message_output[6], message_output[7], message_output[8], message_output[9],
                message_output[10], message_output[11], message_output[12], message_output[13]);
        printf("\nFAILED: Message Encode Test 1/2\n");
    }

    char message_output2[MESSAGE_MAX_LEN];
    static Message encodeTest2 = {MESSAGE_ACC, 57203, 0, 0};
    Message_Encode(message_output2, encodeTest2);
    if (strcmp(message_output2, "$ACC,57203*5E\n") == 0) {
        printf("\nPASSED: Message Encode Test 2/2\n");
    } else {
        printf("\nEncoded Message: %c%c%c%c%c%c%c%c%c%c%c%c%c%c", message_output2[0], message_output2[1],
                message_output2[2], message_output2[3], message_output2[4], message_output2[5],
                message_output2[6], message_output2[7], message_output2[8], message_output2[9],
                message_output2[10], message_output2[11], message_output2[12], message_output2[13]);
        printf("\nFAILED: Message Encode Test 2/2\n");
    }
    
    
    /* TEST FOR PARSE MESSAGE */
    const char payloadParse[] = "RES,4,8,1";
    const char checksumParse[] = "55\n";
    BB_Event parse_event;
    parse_event.param0 = 0;
    parse_event.param1 = 0;
    parse_event.param2 = 0;
    parse_event.type = BB_EVENT_NO_EVENT;
    
    
    
    //Message_ParseMessage(payloadParse, checksumParse, &parse_event);
    //printf("%u", parse_event.param0);
    
    
    int dog = Message_ParseMessage(payloadParse, checksumParse, &parse_event);
    
    if(dog == SUCCESS){
        
        printf("ParseMessage worked 1/2\n");
        
    } else {
        
        printf("Failed");
        
    }
    
    const char payloadParse2[] = "REV,4,8,4";
    const char checksumParse2[] = "55\n";
    dog = Message_ParseMessage(payloadParse2, checksumParse2, &parse_event);
    
    if(dog == SUCCESS){
        
        printf("ParseMessage worked 2/2\n");
        
    } else {
        
        printf("Failed\n");
        
    }
    
    unsigned char test69 = '$';
    BB_Event test70;
    
    dog = Message_Decode(test69, &test70);
    
    if(dog == SUCCESS){
        
        printf("DecodeMessage worked 1/2\n");
        
    } else {
        
        printf("Failed\n");
        
    }
    
    test69 = 'C';
    
    dog = Message_Decode(test69, &test70);
    
    if(dog == SUCCESS){
        
        printf("DecodeMessage worked 2/2\n");
        
    } else {
        
        printf("Failed\n");
        
    }
    
    
    
     printf("END OF TEST\n");
    
    
    
    
    while(1);
}