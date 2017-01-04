/**
 * FencingHardwareLayer.h
 *
 * Created on: 12/31/2016
 *  Author(s): Benjamin Maitland and Samuel Sternklar
 */

//Platform defines will be dealt with as compiler defines
//Platform code should be written in an ifdef and the name of your platform in allcaps
//For example a Raspberry Pi will be RASPBERRY_PI, and an arduino will be ARDUINO

#ifndef FENCING_HARDWARE_LAYER_H
#define FENCING_HARDWARE_LAYER_H

#define MICROSECONDS(X) X
#define MILLISECONDS(X) X * 1000
#define SECONDS(X) X * 1000 * 1000

#ifdef ARDUINO
typedef unsigned long FencingClock;
#elif RASPBERRY_PI
typedef int FencingClock;
#endif

#ifndef __cplusplus
typedef char bool;
#define false 0
#define true 1
#endif

enum Pins
{
	IPIN_LEFT_A,
	IPIN_LEFT_B,
	IPIN_LEFT_C,
	IPIN_LEFT_FAULT,
	IPIN_LEFT_TARGET,
	IPIN_LEFT_OFF_TARGET,
	IPIN_RIGHT_A,
	IPIN_RIGHT_B,
	IPIN_RIGHT_C,
	IPIN_RIGHT_FAULT,
	IPIN_RIGHT_TARGET,
	IPIN_RIGHT_OFF_TARGET,
	IPIN_FLOOR,
	IPIN_WEAPON_BUTTON,
	OPIN_LEFT_FAULT,
	OPIN_RIGHT_FAULT,
	OPIN_LEFT_TARGET,
	OPIN_RIGHT_TARGET,
	OPIN_LEFT_OFF_TARGET,
	OPIN_RIGHT_OFF_TARGET,
	OPIN_BUZZER,
	OPIN_WEAPON_EPEE,
	OPIN_WEAPON_FOIL,
	OPIN_WEAPON_SABER,
};

/**
 * Initializes all GPIO pins defined above
 */
void InitializeIO();
/**
 * Gets all pin states
 */
int GetDigitalPinStates();
/**
 * Gets one pin state
 */
int GetDigitalPinState(Pins pin);
/**
 * Sets all pin states defined as OPIN
 */
void SetDigitalPinStates(Pins outputPinStates);
/**
 * Sets one OPIN state.
 */
void SetPinState(Pins outputPin, int state);
/**
 * Instructs the device to wait for the specified time in microseconds
 */
void Wait(FencingClock timeToWait);
/**
 * Gets the current fencing time in microseconds.
 */
FencingClock PlatformClock();

#endif
