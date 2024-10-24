**Line Following Robot**

This repository contains the complete project for building and programming a Line Following Robot using an STM32 microcontroller and an H-bridge motor controller. The robot follows a black line by reading inputs from two infrared (IR) sensors and adjusting the motor speeds accordingly.

## Project Overview

This project demonstrates the use of PWM signals to control motors via an H-bridge, based on sensor input. The STM32 microcontroller is responsible for reading the IR sensor data and using the inputs to decide how the robot should move (e.g., forward, turn left, turn right, stop).

## Key Features:

	•	Uses PWM (Pulse Width Modulation) for motor speed control.
	•	Infrared (IR) sensors detect the black line on the surface.
	•	Motor control is handled via an L298 H-Bridge.
	•	Displays the movement state (forward, left, right, stop) on an LCD screen.
	•	Configurable for different duty cycles to fine-tune motor speed.

## Hardware Components:

	•	STM32 Microcontroller (with STM32CubeIDE or similar)
	•	L298 H-Bridge for motor control
	•	Two IR sensors (left and right) to detect the line
	•	Two DC motors (left and right wheels)
	•	LCD Display for showing the robot’s movement state
	•	Robot chassis and wheels
	•	Power supply

## Software Components:

	•	C code to handle the initialization of GPIO pins, timers, and interrupts on the STM32.
	•	PWM configuration to control motor speeds.
	•	Logic to determine the robot’s movement based on IR sensor data.
