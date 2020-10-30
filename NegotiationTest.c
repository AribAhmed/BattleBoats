#include "Negotiation.h"

#include "BOARD.H"

#include <xc.h>

#include <stdio.h>

#include <math.h>

int main() {
    BOARD_Init();

    printf("\nWelcome to Negotiation Test, compiled on %s %s \n", __DATE__, __TIME__); 

    NegotiationData test1;
    int result;

    test1 = 3;

    test1 = NegotiationHash(test1);


    if (test1 == 9) {

        printf("Hash works TEST 1/2\n");

    } else {

        printf("Hash does not work\n");

    }



   NegotiationData test2;

    test2 = 12345; //random number

    test2 = NegotiationHash(test2);

    if (test2 == 43182) { //random number

        printf("Hash works TEST 2/2\n");

    } else {

        printf("Hash did not work\n");


    }

    
    printf("Testing NegotiationVerify\n"); //verify check

    NegotiationData test69 = 3;
    
    result = NegotiationVerify(test69, test1);

    
    if (result == TRUE){
        
        printf("NegotiationVerify Works TEST 1/2\n");
        
    } else if (result == FALSE){
        
        printf("NegotiationVerify Does not Work\n");
        
    }
    
    int result2;
    
    NegotiationData test96 = 12345;
    
    result2 = NegotiationVerify(test96, test2);
    
    if (result2 == TRUE){
        
        printf("NegotiationVerify Works TEST 2/2\n");
        
    } else if (result2 == FALSE){
        
        printf("NegotiationVerify Does not Work\n");
        
    }
    
    
    
    
    printf("Testing Coin Flip...\n"); //coin flip check

    NegotiationData test6, test7;
    NegotiationOutcome test9;


    test6 = 3213;
    test7 = 5623;

    test6 = NegotiationHash(test6);
    test7 = NegotiationHash(test7);

    test9 = NegotiateCoinFlip(test6, test7);

    if (test9 == HEADS) {

        printf("Heads was flipped\n");

    } else if (test9 == TAILS) {

        printf("Tails was flipped\n");

    }


    printf("Testing Coin Flip 2...\n");
    NegotiationData test10, test11;
    NegotiationOutcome test12;

    test10 = 1234;
    test11 = 5678;

    test10 = NegotiationHash(test10);
    test11 = NegotiationHash(test11);
    test12 = NegotiateCoinFlip(test10, test11);


    if (test12 == HEADS) {

        printf("Heads was flipped\n");

    } else if (test12 == TAILS) {

        printf("Tails was flipped\n");

    }
    
    
    while (1);
}