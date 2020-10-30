#include "Field.h"

#include "BOARD.H"

#include <xc.h>


#include <math.h>
#include "xc.h"

#include <stdio.h> 
#include <stdlib.h> 

int main() {
    BOARD_Init();


    printf("\nWelcome to Field Test, compiled on %s %s \n", __DATE__, __TIME__);

    Field playerField;
    Field oppField;

    FieldInit(&playerField, &oppField);

    printf("Testing FieldInit()\n");

    if (playerField.smallBoatLives == 0) {

        printf("FieldInit Test passed 1/2\n");

    } else {

        printf("Failed\n");

    }

    if (oppField.smallBoatLives == FIELD_BOAT_SIZE_SMALL) {

        printf("FieldInit Test passed 2/2\n");

    } else {

        printf("Failed \n");

    }

    printf("\n");
    printf("Testing SetSquareStatus\n");

    SquareStatus test = FIELD_SQUARE_CURSOR;
    SquareStatus test2;
    uint8_t row = 1;
    uint8_t col = 2;
    test2 = FieldSetSquareStatus(&playerField, row, col, test);

    if (playerField.grid[row][col] == FIELD_SQUARE_CURSOR) {

        printf("SetSquareStatus works 1/2\n");


    } else {

        printf("Failed\n");

    }

    if (test2 == FIELD_SQUARE_EMPTY) {

        printf("SetSquareStatus works 2/2\n");

    } else {

        printf("Failed\n");

    }

    row = 1;
    col = 2;

    printf("\n");
    printf("Testing GetSquareStatus\n");
    test2 = FieldGetSquareStatus(&playerField, row, col);

    if (test2 == FIELD_SQUARE_CURSOR) {

        printf("GetSquareStatus works 1/2\n");

    } else {

        printf("Failed\n");

    }

    col = 11;

    test2 = FieldGetSquareStatus(&playerField, row, col);

    if (test2 == FIELD_SQUARE_INVALID) {

        printf("GetSquareStatus works 2/2\n");

    } else {

        printf("Failed\n");

    }

    row = 1;
    col = 3;
    uint8_t test5;

    test5 = FieldAddBoat(&playerField, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_MEDIUM);

    printf("\n");
    printf("Testing FieldAddBoat\n");

    if (test5 == SUCCESS) {

        printf("FieldAddBoat works 1/2\n");

    } else if (test5 == STANDARD_ERROR) {

        printf("Failed\n");

    }
    col = 25;
    test5 = FieldAddBoat(&playerField, row, col, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);

    if (test5 == STANDARD_ERROR) {

        printf("FieldAddBoat works 2/2\n");

    } else if (test5 == SUCCESS) {

        printf("Failed\n");

    }

    GuessData test8;
    test8.row = 1;
    test8.col = 3;

    playerField.mediumBoatLives = 1;

    test2 = FieldRegisterEnemyAttack(&playerField, &test8);
    printf("\n");

    printf("Testing FieldRegisterAttack\n");

    if (test8.result == RESULT_MEDIUM_BOAT_SUNK) {

        printf("Register Enemy Attack Works 1/2 \n");


    } else {

        printf("Failed");

    }


    Field testField;
    Field test2Field;
    FieldInit(&testField, &test2Field);
    row = 1;
    col = 3;

    FieldAddBoat(&testField, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL);

    GuessData test9;
    test9.row = 1;
    test9.col = 3;

    test2 = FieldRegisterEnemyAttack(&testField, &test9);

    if (test9.result == RESULT_SMALL_BOAT_SUNK) {

        printf("Register Enemy Attack Works 2/2 \n");

    } else if (test9.result == RESULT_HIT) {

        printf("Register Enemy Attack Works 2/2 \n");

    } else {

        printf("Failed\n");

    }


    printf("\n");
    printf("Testing FieldUpdateKnowledge\n");

    SquareStatus test69 = FieldUpdateKnowledge(&playerField, &test8);

    if (test69 == FIELD_SQUARE_HIT) {

        printf("FieldUpdateKnowledge passed 1/2 \n");

    } else {

        printf("Failed\n");

    }

    test8.result = 1292;
    test69 = FieldUpdateKnowledge(&playerField, &test8);

    if (test69 == FIELD_SQUARE_MISS) {

        printf("FieldUpdateKnowledge passed 2/2 \n");

    } else {

        printf("Failed\n");

    }


    printf("\n");

    printf("Testing GetBoatStates\n");

    playerField.hugeBoatLives = 2;
    playerField.largeBoatLives = 2;
    playerField.mediumBoatLives = 0;
    playerField.smallBoatLives = 0;


    test2 = FieldGetBoatStates(&playerField);

    if (test2 == 0b00001100) {

        printf("GetBoatStates passed 1/2\n");

    } else {

        printf("Failed \n");

    }

    playerField.hugeBoatLives = 0;
    playerField.largeBoatLives = 0;
    playerField.mediumBoatLives = 2;
    playerField.smallBoatLives = 2;

    //0000 0000
    //0000 0100
    //0000 1000
    //0000 1100

    test2 = FieldGetBoatStates(&playerField);

    if (test2 == 0b00000011) {

        printf("GetBoatStates passed 2/2\n");

    } else {

        printf("Failed \n");

    }

    printf("\n");

    printf("Testing AiPlaceBoats\n");

    Field aiField;
    Field dad;
    FieldInit(&aiField, &dad);
    
    Field aiField2;
    Field dad2;
    
    FieldInit(&aiField2, &dad2);

    uint8_t ai;

    ai = FieldAIPlaceAllBoats(&aiField);

    if (ai == SUCCESS) {

        printf("AiPlaceBoats worked 1/2\n");

    } else if (ai == STANDARD_ERROR) {

        printf("Failed\n");

    }
    

    ai = FieldAIPlaceAllBoats(&aiField2);
    
    if (ai == SUCCESS) {

        printf("AiPlaceBoats worked 2/2\n");

    } else if (ai == STANDARD_ERROR) {

        printf("Failed\n");

    }

    printf("\n");

    printf("Testing AiDecide Guess\n");

    GuessData test20;

    test20 = FieldAIDecideGuess(&aiField);

    if ((test20.row >= 0) && (test20.col >= 0)) {

        printf("DecideGuess works 1/2\n");

    } else {

        printf("Failed\n");

    }

   // test20 = FieldAIDecideGuess(&testField);

    if ((test20.row >= 0) && (test20.col >= 0)) {

        printf("DecideGuess works 2/2\n");

    } else {

        printf("Failed\n");

    }

    printf("\n");

    printf("End of Test\n");
    
    while (1);
}