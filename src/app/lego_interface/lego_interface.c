/**
 * @file
 * @mainpage Lego Interface Module.
 * This module implements a driver to the LEGO interface module hardware.
 * The functionality are:
 *  - Handles power supply to all sensor types (light, touch and rotation).
 *  - Handles reading of actual sensor values for up to 8 LEGO sensors.
 *  - Controls rotation direction and speed for up to 4 LEGO motors.
 *
 *  \note before using any of the functions in this module the init_lego_interface function
 *   must be called!
 *  \version 1.0.2
 *  \date 2010-05-03
 *  \author Ib Havn, iha@viauc.dk VIA University College.
 */
#include <includes.h>

static INT8U sensor_channel = 0;
static INT16U sensor_result[8];

// Motor control
// braked = 1 when braked else 0
static INT8S motor_braked[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

/**
 ********************************************************************************************************
 * Initialize the LEGO Interface driver.
 * \note This must be called before any of the LEGO interface functions are called.
 * Timer 3 is used to time the interface to the sensors.
 * Timer 1, 4 and 5 are used to control the H-Bridges for Motor-control.
 * PORTF is used to interface to LEGO sensors.
 * PORTB pin 5, 6 and 7, PORTH pin 3, 4 and 5, PORTL pin 3 & 4 are all used to control the H-Bridges.
 *********************************************************************************************************
 */
void init_lego_interface(void) {
	// Setup ADC
	// ref=AVCC
	ADMUX |= _BV(REFS0);
	// Enable ADC
	ADCSRA |= _BV(ADEN);
	// Enable ADC interrupt
	ADCSRA |= _BV(ADIE);
	// Prescaler = 1/64 => 156250hZ
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1);
	// Disable digital pin inputs on PORTF
	DIDR0 = 0xFF;

	// Timer 3 setup to TOP = 100mS
	// Mode 4: CTC => TOP = OCR3A
	TCCR3B |= _BV(WGM32);
	// TOP = OCR3A = 15624 => 100 ms @ CLKio = 10Mhz and prescaler = 1/8
	OCR3A = 15624;
	// Enable OCR3A match interrupt. int every 100ms
	TIMSK3 |= _BV(OCIE3A);
	// reset timer reg
	TCNT3 = 0;
	// Prescaler = 1/64 => 100mS @ CLKio = 10Mhz and start timer 3
	TCCR3B |= _BV(CS31) | _BV(CS30);

	// PWM setup for motor control
	// Timer 1 setup to fast PWM for controlling H-Bridge 1/Motor 0 and half of H-Bridge 2/Motor 1
	// setup to output OC1A/PB5, OC1B/PB6 and OC1C/PB7 pins to output
	DDRB |= _BV(DDB5) | _BV(DDB6) | _BV(DDB7);
	// PB5 = PB6 = PB7 to 0 to be prepared for brake condition
	PORTB &= ~(_BV(PB5) | _BV(PB6) | _BV(PB7));
	// Mode 5: Fast PWM 8-bit, TOP = 0xFF
	TCCR1A |= _BV(WGM10);
	TCCR1B |= _BV(WGM12);
	// Compare output mode: Set OC1A on BOTTOM and clear OC1A, OC1B, OC1C on compare match
	TCCR1A |= _BV(COM1A1) | _BV(COM1B1) | _BV(COM1C1);
	// set H-Bridge inputs to tri-state output => Motor free run
	OCR1A = OCR1B = OCR1C = 0xff;
	// Prescaler 1/1 and start timer => PWM Freq = 39,216 khz @ CLKio = 10Mhz
	TCCR1B |= _BV(CS10);

	// Timer 4 setup to fast PWM for controlling half of H-Bridge 2/Motor 1 and H-Bridge 3/Motor 2
	// setup to output OC4A/PH3, OC4B/PH4 and OC4C/PH5 pins to output
	DDRH |= _BV(DDH3) | _BV(DDH4) | _BV(DDH5);
	// PH3, PH4 and PH5 to 0 to be prepared for brake condition
	PORTH &= ~(_BV(PH3) | _BV(PH4) | _BV(PH5));
	// Mode 5: Fast PWM 8-bit, TOP = 0xFF
	TCCR4A |= _BV(WGM40);
	TCCR4B |= _BV(WGM42);
	// Compare output mode: Set OCx on BOTTOM and clear OC1C, OC4A, O41B,  on compare match
	TCCR4A |= _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1);
	// set H-Bridge inputs to tri-state output => Motor free run
	OCR4A = OCR4B = OCR4C = 0xff;
	// Prescaler 1/1 and start timer => PWM Freq = 39,216 khz @ CLKio = 10Mhz
	TCCR4B |= _BV(CS40);

	// Timer 5 setup to fast PWM for controlling H-Bridge 4/Motor 3
	// setup to output OC5A/PL3 and OC5B/PL4 pins to output
	DDRL |= _BV(DDL3) | _BV(DDL4);
	// PL3 and PL4 to 0 to be prepared for brake condition
	PORTL &= ~(_BV(PL3) | _BV(PL4));
	// Mode 5: Fast PWM 8-bit, TOP = 0xFF
	TCCR5A |= _BV(WGM50);
	TCCR5B |= _BV(WGM52);
	// Compare output mode: Set OCx on BOTTOM and clear OC5A, OC5B on compare match
	TCCR5A |= _BV(COM5A1) | _BV(COM5B1);
	// set H-Bridge inputs to tri-state output => Motor free run
	OCR5A = OCR5B = 0xff;
	// Prescaler 1/1 and start timer => PWM Freq = 39,216 khz @ CLKio = 10Mhz
	TCCR5B |= _BV(CS50);
}

/**
 ********************************************************************************************************
 *	Read the actual value from the touch sensor connected to the specified sensor input.
 *	\param sensor_no specifies the sensor input on the interface board [0..7].
 *	\return the 10-bit value read on the sensor, or -1 if sensor_no out of range [0..7].
 *********************************************************************************************************
 */
INT16S touch_sensor(INT8U sensor_no) {
	if (sensor_no < 8) {
		return sensor_result[sensor_no];
	} else {
		return -1;
	}
}

/**
 ********************************************************************************************************
 *	Read the actual value from the rotation sensor connected to the specified sensor input.
 *	\param sensor_no specifies the sensor input on the interface board [0..7].
 *	\return the 10-bit value read on the sensor, or -1 if sensor_no out of range [0..7].
 *********************************************************************************************************
 */
INT16S rotation_sensor(INT8U sensor_no) {
	if (sensor_no < 8) {
		return sensor_result[sensor_no];
	} else {
		return -1;
	}
}

/**
 ********************************************************************************************************
 *	Read the actual value from the light sensor connected to the specified sensor input.
 *	\param sensor_no specifies the sensor input on the interface board [0..7].
 *	\return the 10-bit value read on the sensor, or -1 if sensor_no out of range [0..7].
 *********************************************************************************************************
 */
INT16S light_sensor(INT8U sensor_no) {
	if (sensor_no < 8) {
		return sensor_result[sensor_no];
	} else {
		return -1;
	}
}

/**
 ********************************************************************************************************
 *	Sets the speed and direction of the motor connected to the specified motor output/H-Bridge.
 *	\param motor_no specifies the motor output on the interface board [0..3].
 *	\param speed in percent from -100 to 100 where -100 is full speed CCW, and 100 is full speed CW.\n
 *   0 will release the motor/free run. If the motor is braked the brake will automatically be released
 *   when this function is called.
 *	\see brake_motor
 *********************************************************************************************************
 */
void motor_speed(INT8U motor_no, INT8S speed) {
	if ((speed >= -100) && (speed <= 100)) {
#if OS_CRITICAL_METHOD == 3                                             /* Allocate storage for CPU status register                 */
		OS_CPU_SR cpu_sr = 0;
#endif

		OS_ENTER_CRITICAL();

		INT8U ocrx_value = 255- (abs(speed) * 255 / 100);

		if (motor_braked[motor_no]) {
			motor_braked[motor_no] = 0;
		}

		switch (motor_no) {
		case 0:
			if (speed > 0) {
				// CW
				// New value in OCR1A register
				OCR1A = ocrx_value;
				// Set OC1B high by setting OCR1B to TOP
				OCR1B = 0xFF;
				//TCCR1A &= ~_BV(COM1B1);
				// set OC1A & OC1B to Clear on compare
				TCCR1A |= _BV(COM1A1) | _BV(COM1B1);
			} else if (speed == 0) {
				// set both H-Bridge inputs to tri-state output => Motor free run
				OCR1A = OCR1B = 0xFF;
				// Compare output mode: Set OC1A on BOTTOM and clear OC1A, OC1B on compare match
				TCCR1A |= _BV(COM1A1) | _BV(COM1B1);
			} else {
				// CCW
				// New value in OCR1B register
				OCR1B = ocrx_value;
				// Set OC1A high by setting OCR1A to TOP
				OCR1A = 0xFF;
				//TCCR1A &= ~_BV(COM1A1);
				// set OC1A & OC1B to Clear on compare
				TCCR1A |= _BV(COM1A1) | _BV(COM1B1);			}
			OS_EXIT_CRITICAL();
			break;

		case 1:
			if (speed > 0) {
				// CW
				// New value in OCR1C register
				OCR1C = ocrx_value;
				// Set OC4A high by setting OCR4A to TOP
				OCR4A = 0xFF;
				// set OC1C & OC4A to Clear on compare
				TCCR1A |= _BV(COM1C1);
				TCCR4A |= _BV(COM4A1);
			} else if (speed == 0) {
				// set both H-Bridge inputs to tri-state output => Motor free run
				OCR1C = OCR4A = 0xFF;
				// Compare output mode: Set OC1C on BOTTOM and clear OC1C, OC4A on compare match
				TCCR1A |= _BV(COM1C1);
				TCCR4A |= _BV(COM4A1);
			} else {
				// CCW
				// New value in OCR4A register
				OCR4A = ocrx_value;
				// Set OC1C high by setting OCR1C to TOP
				OCR1C = 0xFF;
				// set OC1C & OC4A to Clear on compare
				TCCR1A |= _BV(COM1C1);
				TCCR4A |= _BV(COM4A1);
			}
			OS_EXIT_CRITICAL();
			break;

		case 2:
			if (speed > 0) {
				// CW
				// New value in OCR4B register
				OCR4B = ocrx_value;
				// Set OC4C high by setting OCR4C to TOP
				OCR4C = 0xFF;
				// set OC4B & OC4C to Clear on compare
				TCCR4A |= _BV(COM4B1) | _BV(COM4C1);
			} else if (speed == 0) {
				// set both H-Bridge inputs to tri-state output => Motor free run
				OCR4B = OCR4C = 0xFF;
				// Compare output mode: Set OC4B, OC4C on BOTTOM and clear OC4B, OC4C on compare match
				TCCR4A |= _BV(COM4B1) | _BV(COM4C1);
			} else {
				// CCW
				// New value in OCR4C register
				OCR4C = ocrx_value;
				// Set OC4B high by setting OCR4B to TOP
				OCR4B = 0xFF;
				// set OC4B & OC4C to Clear on compare
				TCCR4A |= _BV(COM4B1) | _BV(COM4C1);
			}
			OS_EXIT_CRITICAL();
			break;

		case 3:
			if (speed > 0) {
				// CW
				// New value in OCR5A register
				OCR5A = ocrx_value;
				// Set OC5B high by setting OCR5B to TOP
				OCR5B = 0xFF;
				// set OC5A & OC5B to Clear on compare
				TCCR5A |= _BV(COM5A1) | _BV(COM5C1);
			} else if (speed == 0) {
				// set both H-Bridge inputs to tri-state output => Motor free run
				OCR5A = OCR5B = 0xFF;
				// Compare output mode: Set OC5A, OC5B on BOTTOM and clear OC5A, OC5B on compare match
				TCCR5A |= _BV(COM5A1) | _BV(COM5B1);
			} else {
				// CCW
				// New value in OCR5B register
				OCR5B = ocrx_value;
				// Set OC5A high by setting OCR5A to TOP
				OCR5A = 0xFF;
				// set OC5A & OC5B to Clear on compare
				TCCR5A |= _BV(COM5A1) | _BV(COM5C1);
			}
			OS_EXIT_CRITICAL();
			break;

		default:
			OS_EXIT_CRITICAL();
		}
	}
}

/**
 ********************************************************************************************************
 * Brakes the motor connected to the specified motor output/H-Bridge. The brake is established by
 * shorting the motor terminals with the H-Bridge.
 *	\param motor_no specifies the motor output on the interface board [0..3].
 *********************************************************************************************************
 */
void brake_motor(INT8U motor_no) {
#if OS_CRITICAL_METHOD == 3                                             /* Allocate storage for CPU status register                 */
	OS_CPU_SR cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();

	if (!motor_braked[motor_no]) {
		motor_braked[motor_no] = 1;

		switch (motor_no) {
		case 0:
			// Normal port operation
			TCCR1A &= ~(_BV(COM1A1) | _BV(COM1B1));
			OS_EXIT_CRITICAL();
			break;

		case 1:
			// Normal port operation
			TCCR1A &= ~_BV(COM1C1);
			TCCR4A &= ~_BV(COM4A1);
			OS_EXIT_CRITICAL();
			break;

		case 2:
			// Normal port operation
			TCCR4A &= ~(_BV(COM4B1) | _BV(COM4C1));
			OS_EXIT_CRITICAL();
			break;

		case 3:
			// Normal port operation
			TCCR5A &= ~(_BV(COM5A1) | _BV(COM5B1));
			OS_EXIT_CRITICAL();
			break;

		default:
			OS_EXIT_CRITICAL();
		}
	}
}

/*
 *********************************************************************************************************
 *	Handles the timing for start of the scanning of the sensor inputs via the ADC.
 *	First all sensors are powered of. Then the first conversion on ADC-channel/mux 0 is started.
 *********************************************************************************************************
 */
ISR(TIMER3_COMPA_vect)
{
	// Setup PORTF to input => means power off to sensors
	DDRF = 0x00;
	PORTF = 0x00;

	// Start conversion on next channel
	ADMUX = (ADMUX & 0b11111000) + sensor_channel;
	// Start conversion
	ADCSRA |= _BV(ADSC);
}

/*
 ********************************************************************************************************
 * Take the result from the ADC when a conversion is finished. Set the ADC-channel/mux to the next
 * sensor input and starts a new ADC conversion. If it was the last sensor that was scanned then
 * all sensors are powered on again.
 *********************************************************************************************************
 */
ISR(ADC_vect)
{
	sensor_result[sensor_channel] = ADC;

	if (sensor_channel < 7) {
		ADMUX = (ADMUX & 0b11111000) + ++sensor_channel;
		// Start conversion
		ADCSRA |= _BV(ADSC);
	} else {
		// power on all sensors
		DDRF = 0xFF;
		PORTF = 0xFF;

		// Prepare next scan
		sensor_channel = 0;
	}
}
