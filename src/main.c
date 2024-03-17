// Written by:
// Date created:
// Programmed in: STM32CubeIDE
// Target: STM32F051
// Description:

// DEFINES AND INCLUDES


#define STM32F051
#include "stm32f0xx.h"
#include "lcd_stm32f0.h"
#define HIGH 1
#define LOW 0

// GLOBAL VARIABLES
enum motion{back=-1, stop, forward, right, left};
struct robot_status{
	enum motion movement;
	_Bool ir_left;
	_Bool ir_right;
} robot_state;


// GLOBAL CONSTANTS


// FUNCTION DECLARATIONS
void main(void);


// MAIN FUNCTION
void main(void)
{
	// Power on phase
	// Robot_state initialised


	robot_state.movement = stop;
	robot_state.ir_left = LOW;
	robot_state.ir_right = LOW;

	while(1){

	}
}

// FUNCTION DEFINITIONS
