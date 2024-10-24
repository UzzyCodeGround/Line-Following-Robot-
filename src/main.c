// Initialize Timer 3 for PWM control of the motors
void init_TIM3(void){ 
    // Enable the clock for GPIOB
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN; 

    // Enable the clock for Timer 3
    RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN; 

    // Set PB0, PB1, PB4, and PB5 to alternate function mode for PWM
    GPIOB -> MODER |= GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1; 
    
    // Assign alternate functions to PB0, PB1, PB4, PB5 for timer output (AF1 for TIM3)
    GPIOB -> AFR[0] |= (0x01 << (0*4)) | (0x01 << (1*4)) | (0x01 << (4*4)) | (0x01 << (5*4)); 
    
    // Set prescaler and auto-reload for timer to define the PWM frequency and resolution
    TIM3 -> PSC = 31;  // Prescaler value
    TIM3 -> ARR = 99;  // Auto-reload value for PWM period
    
    // Set initial duty cycle for each PWM channel
    TIM3 -> CCR1 = 50;  // Channel 1: 50% duty cycle
    TIM3 -> CCR2 = 50;  // Channel 2: 50% duty cycle (inverse of Channel 1)
    TIM3 -> CCR3 = 50;  // Channel 3: 50% duty cycle
    TIM3 -> CCR4 = 50;  // Channel 4: 50% duty cycle (inverse of Channel 3)
    
    // Configure output compare mode for PWM on Channel 1
    TIM3 -> CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE; 
    TIM3 -> CCER |= TIM_CCER_CC1E;  // Enable Channel 1 output

    // Configure output compare mode for PWM on Channel 2
    TIM3 -> CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;
    TIM3 -> CCER |= TIM_CCER_CC2E;  // Enable Channel 2 output

    // Configure output compare mode for PWM on Channel 3
    TIM3 -> CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;
    TIM3 -> CCER |= TIM_CCER_CC3E;  // Enable Channel 3 output

    // Configure output compare mode for PWM on Channel 4
    TIM3 -> CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;
    TIM3 -> CCER |= TIM_CCER_CC4E;  // Enable Channel 4 output

    // Enable Timer 3
    TIM3 -> CR1 |= TIM_CR1_CEN; 
}

// Initialize Timer 14 for interrupt-based timing (every 50ms)
void init_TIM14(void){ 
    // Enable the clock for Timer 14
    RCC -> APB1ENR |= RCC_APB1ENR_TIM14EN; 

    // Set the prescaler value to get a 50ms interrupt
    TIM14 -> PSC = 47999;  // Prescaler value
    TIM14 -> ARR = 49;  // Auto-reload value for 50ms

    // Enable the update interrupt event
    TIM14 -> DIER |= TIM_DIER_UIE; 

    // Enable Timer 14
    TIM14 -> CR1 |= TIM_CR1_CEN; 

    // Enable the Timer 14 interrupt in the NVIC (Nested Vector Interrupt Controller)
    NVIC_EnableIRQ(TIM14_IRQn); 
}

// Interrupt handler for Timer 14 (triggered every 50ms)
void TIM14_IRQHandler(void){ 
    // Clear the update interrupt flag
    TIM14 -> SR &= ~TIM_SR_UIF;

    // Read IR sensor states from GPIO port B and update robot state
    robot_state.ir_left = ((GPIOB -> IDR) & GPIO_IDR_6);  // Check left IR sensor
    robot_state.ir_right = ((GPIOB -> IDR) & GPIO_IDR_7); // Check right IR sensor

    // Set a base duty cycle for motor control
    uint8_t DC = 70;

    // Evaluate IR sensor readings and adjust motor outputs accordingly
    if (robot_state.ir_left && robot_state.ir_right) {
        // Both sensors detect a line, stop the robot
        robot_state.movement = stop;
        TIM3 -> CCR1 = 50; // Set duty cycle to 50% to stop motors
        TIM3 -> CCR2 = 50;
        TIM3 -> CCR3 = 50;
        TIM3 -> CCR4 = 50;
    } 
    else if (!robot_state.ir_left && !robot_state.ir_right){
        // No sensors detect a line, move forward
        robot_state.movement = forward;
        TIM3 -> CCR1 = DC;    // Set left wheel speed
        TIM3 -> CCR2 = 100-DC; // Set right wheel speed (mirrored setup)
        TIM3 -> CCR3 = DC;    
        TIM3 -> CCR4 = 100-DC; 
    } 
    else if (!robot_state.ir_left && robot_state.ir_right){
        // Right sensor detects a line, turn right
        robot_state.movement = right;
        TIM3 -> CCR1 = DC; // Left wheel moves forward
        TIM3 -> CCR2 = 100-DC;
        TIM3 -> CCR3 = 50; // Right wheel stationary
        TIM3 -> CCR4 = 50;
    } 
    else if (robot_state.ir_left && !robot_state.ir_right){
        // Left sensor detects a line, turn left
        robot_state.movement = left;
        TIM3 -> CCR1 = 50; // Left wheel stationary
        TIM3 -> CCR2 = 50;
        TIM3 -> CCR3 = DC; // Right wheel moves forward
        TIM3 -> CCR4 = 100-DC;
    }

    // Update the LCD display with the movement state
    lcd_command(CLEAR);  // Clear the screen
    char buffer[16];     // Buffer for display text

    if (robot_state.movement == forward){
        sprintf(buffer, "forward");
    } 
    else if (robot_state.movement == stop){
        sprintf(buffer, "stop");
    } 
    else if (robot_state.movement == left){
        sprintf(buffer, "left");
    } 
    else if (robot_state.movement == right){
        sprintf(buffer, "right");
    }
    
    lcd_putstring(buffer);  // Send movement state to the display
}
