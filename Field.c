#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "Field.h"
#include "Oled.h"
#include "OledDriver.h"
#include "xc.h"


void FieldPrint_UART(Field *own_field, Field * opp_field) {
    printf("Not implemented");
}

void FieldInit(Field *own_field, Field * opp_field) {
    int i, j;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            own_field->grid[i][j] = FIELD_SQUARE_EMPTY;
            opp_field->grid[i][j] = FIELD_SQUARE_UNKNOWN;
        }
    }
    own_field->smallBoatLives = 0;
    own_field->mediumBoatLives = 0;
    own_field->largeBoatLives = 0;
    own_field->hugeBoatLives = 0;

    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
}

SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col) {
    if ((row > FIELD_ROWS) || (row < 0) || (col > FIELD_COLS) || (col < 0)) {
        return FIELD_SQUARE_INVALID;
    } else {
        return f->grid[row][col];
    }
}

SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p) {
    SquareStatus old = f->grid[row][col];
    f->grid[row][col] = p;
    return old;
}

uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type) {
    int m;
    if ((row < 0) || (col < 0)) {
        return STANDARD_ERROR;

    } else {
        switch (boat_type) {
            case FIELD_BOAT_TYPE_SMALL:
                if (dir == FIELD_DIR_EAST) {
                    if ((col > (FIELD_COLS - FIELD_BOAT_SIZE_SMALL))) {
                        return STANDARD_ERROR;
                    } else {
                        for (m = 0; m < FIELD_BOAT_SIZE_SMALL; m++) {
                            if (f->grid[row][col + m] != FIELD_SQUARE_EMPTY) {
                                return STANDARD_ERROR;
                            } else {
                                f->grid[row][col + m] = FIELD_SQUARE_SMALL_BOAT;
                                f->smallBoatLives++;
                            }
                        }
                        return SUCCESS;
                    }
                }
                if (dir == FIELD_DIR_SOUTH) {
                    if ((row > (FIELD_ROWS - FIELD_BOAT_SIZE_SMALL))) {
                        return STANDARD_ERROR;
                    } else {
                        for (m = 0; m < FIELD_BOAT_SIZE_SMALL; m++) {
                            if (f->grid[row + m][col] != FIELD_SQUARE_EMPTY) {
                                return STANDARD_ERROR;
                            } else {
                                f->grid[row + m][col] = FIELD_SQUARE_SMALL_BOAT;
                                f->smallBoatLives++;
                            }
                        }
                        return SUCCESS;
                    }
                }
                break;

            case FIELD_BOAT_TYPE_MEDIUM:
                if (dir == FIELD_DIR_EAST) {
                    if ((col > (FIELD_COLS - FIELD_BOAT_SIZE_MEDIUM))) {
                        return STANDARD_ERROR;
                    } else {
                        for (m = 0; m < FIELD_BOAT_SIZE_MEDIUM; m++) {
                            if (f->grid[row][col + m] != FIELD_SQUARE_EMPTY) {
                                return STANDARD_ERROR;
                            } else {
                                f->grid[row][col + m] = FIELD_SQUARE_MEDIUM_BOAT;
                                f->mediumBoatLives++;
                            }
                        }
                        return SUCCESS;
                    }
                }
                if (dir == FIELD_DIR_SOUTH) {
                    if ((row > (FIELD_ROWS - FIELD_BOAT_SIZE_MEDIUM))) {
                        return STANDARD_ERROR;
                    } else {
                        for (m = 0; m < FIELD_BOAT_SIZE_MEDIUM; m++) {
                            if (f->grid[row + m][col] != FIELD_SQUARE_EMPTY) {
                                return STANDARD_ERROR;
                            } else {
                                f->grid[row + m][col] = FIELD_SQUARE_MEDIUM_BOAT;
                                f->mediumBoatLives++;
                            }
                        }
                        return SUCCESS;
                    }
                }
                break;

            case FIELD_BOAT_TYPE_LARGE:
                if (dir == FIELD_DIR_EAST) {
                    if ((col > (FIELD_COLS - FIELD_BOAT_SIZE_LARGE))) {
                        return STANDARD_ERROR;
                    } else {
                        for (m = 0; m < FIELD_BOAT_SIZE_LARGE; m++) {
                            if (f->grid[row][col + m] != FIELD_SQUARE_EMPTY) {
                                return STANDARD_ERROR;
                            } else {
                                f->grid[row][col + m] = FIELD_SQUARE_LARGE_BOAT;
                                f->largeBoatLives++;
                            }
                        }
                        return SUCCESS;
                    }
                }
                if (dir == FIELD_DIR_SOUTH) {
                    if ((row > (FIELD_ROWS - FIELD_BOAT_SIZE_LARGE))) {
                        return STANDARD_ERROR;
                    } else {
                        for (m = 0; m < FIELD_BOAT_SIZE_LARGE; m++) {
                            if (f->grid[row + m][col] != FIELD_SQUARE_EMPTY) {
                                return STANDARD_ERROR;
                            } else {
                                f->grid[row + m][col] = FIELD_SQUARE_LARGE_BOAT;
                                f->largeBoatLives++;
                            }
                        }
                        return SUCCESS;
                    }
                }
                break;

            case FIELD_BOAT_TYPE_HUGE:
                if (dir == FIELD_DIR_EAST) {
                    if ((col > (FIELD_COLS - FIELD_BOAT_SIZE_HUGE))) {
                        return STANDARD_ERROR;
                    } else {
                        for (m = 0; m < FIELD_BOAT_SIZE_HUGE; m++) {
                            if (f->grid[row][col + m] != FIELD_SQUARE_EMPTY) {
                                return STANDARD_ERROR;
                            } else {
                                f->grid[row][col + m] = FIELD_SQUARE_HUGE_BOAT;
                                f->hugeBoatLives++;
                            }
                        }
                        return SUCCESS;
                    }
                }
                if (dir == FIELD_DIR_SOUTH) {
                    if ((row > (FIELD_ROWS - FIELD_BOAT_SIZE_HUGE))) {
                        return STANDARD_ERROR;
                    } else {
                        for (m = 0; m < FIELD_BOAT_SIZE_HUGE; m++) {
                            if (f->grid[row + m][col] != FIELD_SQUARE_EMPTY) {
                                return STANDARD_ERROR;
                            } else {
                                f->grid[row + m][col] = FIELD_SQUARE_HUGE_BOAT;
                                f->hugeBoatLives++;
                            }
                        }
                        return SUCCESS;
                    }
                }
                break;

          
        }
    }

    return STANDARD_ERROR;
}

SquareStatus FieldRegisterEnemyAttack(Field *f, GuessData *gData) {
    SquareStatus guess = gData->result;
    if (f->grid[gData->row][gData->col] == FIELD_SQUARE_EMPTY || f->grid[gData->row][gData->col] == FIELD_SQUARE_HIT) {
        f->grid[gData->row][gData->col] = FIELD_SQUARE_MISS;
    } else {
        if (f->grid[gData->row][gData->col] == FIELD_SQUARE_SMALL_BOAT) {
            f->smallBoatLives--;
            if (f->smallBoatLives == 0) {
                gData->result = RESULT_SMALL_BOAT_SUNK;
            } else {
                gData->result = RESULT_HIT;
            }
        }
        if (f->grid[gData->row][gData->col] == FIELD_SQUARE_MEDIUM_BOAT) {
            f->mediumBoatLives--;
            if (f->mediumBoatLives == 0) {
                gData->result = RESULT_MEDIUM_BOAT_SUNK;
            } else {
                gData->result = RESULT_HIT;
            }
        }
        if (f->grid[gData->row][gData->col] == FIELD_SQUARE_LARGE_BOAT) {
            f->largeBoatLives--;
            if (f->largeBoatLives == 0) {
                gData->result = RESULT_LARGE_BOAT_SUNK;
            } else {
                gData->result = RESULT_HIT;
            }
        }
        if (f->grid[gData->row][gData->col] == FIELD_SQUARE_HUGE_BOAT) {
            f->hugeBoatLives--;
            if (f->hugeBoatLives == 0) {
                gData->result = RESULT_HUGE_BOAT_SUNK;
            } else {
                gData->result = RESULT_HIT;
            }
        }
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
    }
    return guess;
}

SquareStatus FieldUpdateKnowledge(Field *f, const GuessData *gData) {
    SquareStatus guess = gData->result;
    if (gData->result == RESULT_SMALL_BOAT_SUNK) {
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->smallBoatLives = 0;
    } else if (gData->result == RESULT_MEDIUM_BOAT_SUNK) {
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->mediumBoatLives = 0;
    } else if (gData->result == RESULT_LARGE_BOAT_SUNK) {
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->largeBoatLives = 0;
    } else if (gData->result == RESULT_HUGE_BOAT_SUNK) {
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->hugeBoatLives = 0;
    } else if (gData->result == RESULT_HIT) {
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
    } else {
        f->grid[gData->row][gData->col] = FIELD_SQUARE_MISS;
    }
    return guess;
}

uint8_t FieldGetBoatStates(const Field *f) {
    uint8_t boatStates = 0x00;
    if (f->smallBoatLives != 0) {
        boatStates = boatStates | FIELD_BOAT_STATUS_SMALL; // bitwise OR
    }
    if (f->mediumBoatLives != 0) {
        boatStates = boatStates | FIELD_BOAT_STATUS_MEDIUM; // bitwise OR
    }
    if (f->largeBoatLives != 0) {
        boatStates = boatStates | FIELD_BOAT_STATUS_LARGE; // bitwise OR
    }
    if (f->hugeBoatLives != 0) {
        boatStates = boatStates | FIELD_BOAT_STATUS_HUGE; // bitwise OR
    }
    return boatStates;
}

uint8_t FieldAIPlaceAllBoats(Field *f) {
    int dir = rand() & 0x01;
    int col = rand();
    int row = rand();
    int w;

    /* HUGE BOAT PLACEMENT */
    if (dir == 0) { //FIELD_DIR_EAST
        col = rand() % 4;
        row = rand() % 5;
        for (w = 0; w < FIELD_BOAT_SIZE_HUGE; w++) {
            if (f->grid[row][col + w] != FIELD_SQUARE_EMPTY) {
                col = rand() % 4;
                row = rand() % 5;
                w = 0;
            }
        }
        FieldAddBoat(f, row, col, FIELD_DIR_EAST, FIELD_BOAT_SIZE_HUGE);
    } else if (dir == 1) { //FIELD_DIR_SOUTH
        col = rand() % 9;
        row = 0;
        for (w = 0; w < FIELD_BOAT_SIZE_HUGE; w++) {
            if (f->grid[row + w][col] != FIELD_SQUARE_EMPTY) {
                col = rand() % 9;
                w = 0;
            }
        }
        FieldAddBoat(f, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_SIZE_HUGE);
    } else {
        return STANDARD_ERROR;
    }
    dir = rand() & 0x01;

    /* LARGE BOAT PLACEMENT */
    if (dir == 0) { //FIELD_DIR_EAST
        col = rand() % 5;
        row = rand() % 5;
        for (w = 0; w < FIELD_BOAT_SIZE_LARGE; w++) {
            if (f->grid[row][col + w] != FIELD_SQUARE_EMPTY) {
                col = rand() % 5;
                row = rand() % 5;
                w = 0;
            }
        }
        FieldAddBoat(f, row, col, FIELD_DIR_EAST, FIELD_BOAT_SIZE_LARGE);
    } else if (dir == 1) { //FIELD_DIR_SOUTH
        col = rand() % 9;
        row = rand() & 0x01;
        for (w = 0; w < FIELD_BOAT_SIZE_LARGE; w++) {
            if (f->grid[row + w][col] != FIELD_SQUARE_EMPTY) {
                col = rand() % 9;
                w = 0;
            }
        }
        FieldAddBoat(f, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_SIZE_LARGE);
    } else {
        return STANDARD_ERROR;
    }
    dir = rand() & 0x01;

    /* MEDIUM BOAT PLACEMENT */
    if (dir == 0) { //FIELD_DIR_EAST
        col = rand() % 6;
        row = rand() % 5;
        for (w = 0; w < FIELD_BOAT_SIZE_MEDIUM; w++) {
            if (f->grid[row][col + w] != FIELD_SQUARE_EMPTY) {
                col = rand() % 6;
                row = rand() % 5;
                w = 0;
            }
        }
        FieldAddBoat(f, row, col, FIELD_DIR_EAST, FIELD_BOAT_SIZE_MEDIUM);
    } else if (dir == 1) { //FIELD_DIR_SOUTH
        col = rand() % 9;
        row = rand() % 2;
        for (w = 0; w < FIELD_BOAT_SIZE_MEDIUM; w++) {
            if (f->grid[row + w][col] != FIELD_SQUARE_EMPTY) {
                col = rand() % 9;
                row = rand() % 2;
                w = 0;
            }
        }
        FieldAddBoat(f, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_SIZE_MEDIUM);
    } else {
        return STANDARD_ERROR;
    }
    dir = rand() & 0x01;


    /* SMALL BOAT PLACEMENT */
    if (dir == 0) { //FIELD_DIR_EAST
        col = rand() & 0x07;
        row = rand() % 5;
        for (w = 0; w < FIELD_BOAT_SIZE_SMALL; w++) {
            if (f->grid[row][col + w] != FIELD_SQUARE_EMPTY) {
                col = rand() & 0x07;
                row = rand() % 5;
                w = 0;
            }
        }
        FieldAddBoat(f, row, col, FIELD_DIR_EAST, FIELD_BOAT_SIZE_SMALL);
        return SUCCESS;
    } else if (dir == 1) { //FIELD_DIR_SOUTH
        col = rand() % 9;
        row = rand() & 0x03;
        for (w = 0; w < FIELD_BOAT_SIZE_SMALL; w++) {
            if (f->grid[row + w][col] != FIELD_SQUARE_EMPTY) {
                col = rand() % 9;
                row = rand() & 0x03;
                w = 0;
            }
        }
        FieldAddBoat(f, row, col, FIELD_DIR_EAST, FIELD_BOAT_SIZE_SMALL);
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

int boatHoriz = FALSE;
int decrementFlag = FALSE;

GuessData FieldAIDecideGuess(const Field *f) {

   GuessData guessAI;
    
    int row, col;
    
    row = rand() % 5;
    col = rand() % 9;
    
    guessAI.row = row;
    guessAI.col = col;
    
    return guessAI;

}