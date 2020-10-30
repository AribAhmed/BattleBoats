#include "Agent.h"

#include "BOARD.H"

#include <xc.h>


#include <math.h>
#include "xc.h"

#include <stdio.h> 
#include <stdlib.h> 

int main() {
    BOARD_Init();
    AgentInit();

    printf("\nWelcome to Agent Test, compiled on %s %s \n", __DATE__, __TIME__);

    if (AgentGetState() == AGENT_STATE_START) {
        printf("PASSED: AgentInit()\n");
    }
    AgentState test;
    AgentState test2;

    test = AGENT_STATE_CHALLENGING;


    AgentSetState(test);
    test2 = AgentGetState();
    if (test == test2) {
        printf("PASSED AgentSetState 1/2, AgentGetState1/2\n");
    } else {
        printf("FAILED\n");

    }

    test = AGENT_STATE_ATTACKING;

    AgentSetState(test);
    test2 = AgentGetState();

    if (test2 == AGENT_STATE_ATTACKING) {

        printf("PASSED AgentSetState 2/3, AgentGetState 2/3\n");

    } else {
        printf("FAILED\n");

    }

    printf("Testing AgentRun 1/2 Start\n");

    Message test69;

    static BB_Event testEvent;

    testEvent.type = BB_EVENT_START_BUTTON;

    AgentSetState(AGENT_STATE_START);

    test69 = AgentRun(testEvent);

    if (test69.type == MESSAGE_CHA) {

        printf("AgentRun sends correct message \n");

    } else {

        printf("Failed\n");

    }

    if (test69.param0 > 0) {

        printf("AgentRun hashes a number\n");

    }

    printf("AgentRun test 2/2 challenging\n");

    testEvent.type = BB_EVENT_ACC_RECEIVED;

    AgentSetState(AGENT_STATE_CHALLENGING);

    test69 = AgentRun(testEvent);

    if (test69.type == MESSAGE_REV) {

        printf("AgentRun sends correct message \n");

    } else {

        printf("Failed\n");

    }


    printf("End of test\n");


    while (1);
}