#include "BOARD.h"
#include "Field.h"
#include "Negotiation.h"
#include "Message.h"
#include <xc.h>
#include <stdio.h>
#include "Agent.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Buttons.h"
#include "FieldOled.h"

static Field playerField;
static Field enemyField;
static AgentState state;
int count;
static FieldOledTurn turn;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *      Initializing both fields
 *      Placing boats on your field
 *      Setting the start state of the Agent SM
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * The agent can assume that stdlib's rand() function has been seeded properly, so it is not needed
 * to call srand() inside AgentInit().
 *  */

void AgentInit(void) {

    ButtonsInit(); //idk if this is needed but w/e
    FieldInit(&playerField, &enemyField); //init fields
    FieldAIPlaceAllBoats(&playerField); //place all fields
    count = 0; //turn count?
    state = AGENT_STATE_START; //set the state 
    turn = FIELD_OLED_TURN_NONE;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */

Message AgentRun(BB_Event event) {

    Message test;
    NegotiationData A, B;
    NegotiationData HashA;
    NegotiationOutcome flip;
    GuessData gdata;
    int cheating;
    A = rand();
    HashA = NegotiationHash(A);
    B = rand();
    char endscreen[20];
    char startscreen[20];


    //this whole thing is following the fsm in the lab manual.
    switch (state) {

        case AGENT_STATE_START: //if state statement

            printf("Agent Event Start\n");

            if (event.type == BB_EVENT_START_BUTTON) { //start button

                printf("BB Event Start Button\n");
                //generate A, #a
                A = rand();
                HashA = NegotiationHash(A);
                //send cha
                test.type = MESSAGE_CHA;
                test.param0 = HashA;
                //initialize Fields.
                FieldInit(&playerField, &enemyField);
                FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                event.type = BB_EVENT_CHA_RECEIVED;
                state = AGENT_STATE_CHALLENGING;
                break;

            }

            if (event.type == BB_EVENT_CHA_RECEIVED) { //if challenge recieved

                printf("BB Event Cha Received\n");
                //generate b
                B = rand();
                //send acc
                test.type = MESSAGE_ACC;
                test.param0 = B;

                //initalize fields
                FieldInit(&playerField, &enemyField);
                state = AGENT_STATE_ACCEPTING;
                FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                break;

            }

            if (event.type == BB_EVENT_RESET_BUTTON) {
                printf("BB Event Reset Button\n");

                //reset data
                test.type = MESSAGE_NONE;
                count = 0;
                state = AGENT_STATE_START;

                //display new game message
                sprintf(startscreen, "Press BTN4 to start");
                turn = FIELD_OLED_TURN_NONE;
                AgentInit();
                FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                break;
            }

            break;

        case AGENT_STATE_CHALLENGING:


            printf("In Agent State Challenging\n");

            if (event.type == BB_EVENT_ACC_RECEIVED) {

                printf("BB Event Acc Received\n");
                //send rev
                test.type = MESSAGE_REV;
                test.param0 = A;
                flip = NegotiateCoinFlip(A, B); //coin flips

                if (flip == HEADS) {

                    printf("flip heads\n");
                    state = AGENT_STATE_WAITING_TO_SEND;
                    break;

                }

                if (flip == TAILS) {
                    printf("flip tails\n");
                    state = AGENT_STATE_DEFENDING;
                    break;
                }

            }

            if (event.type == BB_EVENT_RESET_BUTTON) {
                printf("BB Event Reset Button\n");

                //reset data
                test.type = MESSAGE_NONE;
                count = 0;
                state = AGENT_STATE_START;

                //display new game message
                sprintf(startscreen, "Press BTN4 to start");
                turn = FIELD_OLED_TURN_NONE;
                AgentInit();
                FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                break;
            }


            break;

        case AGENT_STATE_ACCEPTING:

            printf("In Agent State Accepting\n");
            if (event.type == BB_EVENT_REV_RECEIVED) {
                printf("BB Event REV Received\n");

                cheating = NegotiationVerify(A, HashA);

                if (cheating == FALSE) {

                    printf("there is cheating\n");
                    state = AGENT_STATE_END_SCREEN;
                    FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                    break;

                } else {

                    flip = NegotiateCoinFlip(A, B);

                    if (flip == HEADS) {

                        printf("flip is heads\n");
                        state = AGENT_STATE_DEFENDING;
                        FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                        break;

                    }

                    if (flip == TAILS) {
                        printf("flip is tails\n");
                        FieldAIDecideGuess(&playerField);
                        //send sho
                        test.type = MESSAGE_SHO;
                        test.param0 = gdata.row;
                        test.param1 = gdata.col;
                        state = AGENT_STATE_ATTACKING;
                        FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                        break;

                    }

                }
            }

            if (event.type == BB_EVENT_RESET_BUTTON) {
                printf("BB Event Reset Button\n");

                //reset data
                test.type = MESSAGE_NONE;
                count = 0;
                state = AGENT_STATE_START;

                //display new game message
                sprintf(startscreen, "Press BTN4 to start");
                turn = FIELD_OLED_TURN_NONE;
                OledDrawString(startscreen);
                OledUpdate();
                break;
            }



            break;

        case AGENT_STATE_ATTACKING:

            printf("In Agent State Attacking\n");
            if (event.type == BB_EVENT_RES_RECEIVED) {
                printf("BB Event RES Received\n");
                //update record
                FieldUpdateKnowledge(&enemyField, &gdata);

                if (FieldGetBoatStates(&enemyField) == 0x0) {
                    printf("Victory\n");
                    sprintf(endscreen, "You win!");
                    OledDrawString(startscreen);
                    OledUpdate();
                    state = AGENT_STATE_END_SCREEN;
                    break;

                } else {

                    printf("Not a victory yet - defending\n");
                    state = AGENT_STATE_DEFENDING;
                    FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                    break;

                }
            }

            if (event.type == BB_EVENT_RESET_BUTTON) {
                printf("BB Event Reset Button\n");

                //reset data
                test.type = MESSAGE_NONE;
                count = 0;
                state = AGENT_STATE_START;

                //display new game message
                sprintf(startscreen, "Press BTN4 to start");
                OledDrawString(startscreen);
                OledUpdate();
                break;
            }



            break;

        case AGENT_STATE_DEFENDING:

            printf("In Agent State Defending\n");
            if (event.type == BB_EVENT_SHO_RECEIVED) {
                printf("BB Event SHO Received\n");
                //updatefield
                FieldUpdateKnowledge(&playerField, &gdata);

                if (FieldGetBoatStates(&playerField) == 0x0) {

                    printf("Defeat\n");
                    sprintf(endscreen, "You lose!");
                    OledDrawString(startscreen);
                    OledUpdate();
                    FieldOledDrawScreen(&playerField, &enemyField, turn, count);

                    state = AGENT_STATE_END_SCREEN;
                    break;

                } else {
                    printf("Didn't lose yet\n");
                    //send res
                    test.type = MESSAGE_RES;
                    test.param0 = gdata.row;
                    test.param1 = gdata.col;
                    test.param2 = gdata.result;
                    state = AGENT_STATE_WAITING_TO_SEND;
                    FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                    break;
                }
            }

            if (event.type == BB_EVENT_RESET_BUTTON) {
                printf("BB Event Reset Button\n");

                //reset data
                test.type = MESSAGE_NONE;
                count = 0;
                state = AGENT_STATE_START;

                //display new game message
                AgentInit();
                FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                break;
            }

            break;

        case AGENT_STATE_WAITING_TO_SEND:

            printf("In Agent State Waiting to Send\n");

            if (event.type == BB_EVENT_MESSAGE_SENT) {
                printf("BB Event Message Sent\n");
                //increment turn count
                count++;
                //decide guess
                gdata = FieldAIDecideGuess(&playerField);
                //send sho
                test.type = MESSAGE_SHO;
                test.param0 = gdata.row;
                test.param1 = gdata.col;
                state = AGENT_STATE_ATTACKING;
                FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                break;
            }

            if (event.type == BB_EVENT_RESET_BUTTON) {
                printf("BB Event Reset Button\n");

                //reset data
                test.type = MESSAGE_NONE;
                count = 0;
                state = AGENT_STATE_START;

                //display new game message
                AgentInit();
                FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                break;
            }


            break;

        case AGENT_STATE_END_SCREEN:
            printf("In Agent State End Screen\n");
            //display message

            if (event.type == BB_EVENT_RESET_BUTTON) {
                printf("BB Event Reset Button\n");
                //reset data
                test.type = MESSAGE_NONE;

                count = 0;
                turn =
                        state = AGENT_STATE_START;

                //display new game message
                AgentInit();
                FieldOledDrawScreen(&playerField, &enemyField, turn, count);
                break;

            }



            break;

        case AGENT_STATE_SETUP_BOATS:

            printf("In Agent State Setup Boats\n");

            break;

    }

    return test;

}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */

AgentState AgentGetState(void) {

    return state; //returns the AgentState

}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState) {

    state = newState; //sets State

}