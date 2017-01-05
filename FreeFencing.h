/**
 * FreeFencing.h
 *
 * Created on: 12/31/2016
 *  Author(s): Samuel Sternklar
 */

//Platform defines will be dealt with as compiler defines
//Platform code should be written in an ifdef and the name of your platform in allcaps
//For example a Raspberry Pi will be RASPBERRY_PI, and an arduino will be ARDUINO
//This might change based on platform defines later

#ifndef FREE_FENCING_H
#define FREE_FENCING_H

#include "FencingHardwareLayer.h"

/**
 *Enum defining which weapon mode we're on
 */
typedef enum Mode
{
	MODE_EPEE,
	MODE_FOIL,
	MODE_SABER
} Mode;

/**
 * wrapper for a time tracker of sorts that gets used for the weapons
 */
typedef struct {
	FencingClock timeStarted;
	bool active;
} FencingTimeTracker;

//TODO: Ideally this could get read from some sort of file later
/**
 * FencingSettings struct, stores all settings info.
 * We have a separate struct for this so we can change it later
 * if we ever read data from a file or something like that
 */
typedef struct {
	FencingClock epeeLockoutTime;
	FencingClock epeeDebounceTime;
	FencingClock saberLockoutTime;
	FencingClock saberDebounceTime;
	FencingClock foilLockoutTime;
	FencingClock foilDebounceTime;
	FencingClock wirePropagationTime;
	FencingClock buzzerTime;
	FencingClock lightTime;
} FencingSettings;

FencingSettings defaultSettings = {
	.epeeLockoutTime = MILLISECONDS(40),
	.epeeDebounceTime = MILLISECONDS(2),
	.saberLockoutTime = MILLISECONDS(170),
	.saberDebounceTime = MICROSECONDS(500),
	.foilLockoutTime = MILLISECONDS(300),
	.foilDebounceTime = MILLISECONDS(14),
	.wirePropagationTime = MICROSECONDS(100),
	.buzzerTime = MILLISECONDS(500),
	.lightTime = MILLISECONDS(500),
};

/**
 * Updates fault light pins each loop
 */
void SetFaultLights();

/**
 * Updates the light pins based on game state
 */
void SetLights();

/**
 * Waits for a very short period of time
 * This is very imprecise
 * Should only be used for sub-millisecond wait timings
 */
void WaitShortTime(FencingClock waitTime);

/**
 * Handles everything that occurs immediately after lockout,
 * like resetting game state, the buzzer, turning off lights, etc.
 */
void HandleLockout();

/**
 * Sets the current weapon mode
 */
void SetMode(Mode newMode);

/**
 * Runs the rules for Epee. Should only be called while Epee mode is active.
 */
void EpeeMode();
//TODO: Foil and Saber
//void FoilMode();
//void SaberMode();

#endif
