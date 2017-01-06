/**
 * FreeFencing.c
 *
 * Created on: 12/31/2016
 *  Author(s): Samuel Sternklar
 */

#include "FreeFencing.h"

FencingSettings settings;
FencingTimeTracker left;
FencingTimeTracker right;
FencingTimeTracker buzzer;
bool leftOffTarget = false;
bool rightOffTarget = false;
bool leftHit = false;
bool rightHit = false;
bool hasLockedOut = false;
Mode mode = MODE_EPEE;
bool currentlyPressingWeaponButton = false;
bool previouslyPressingWeaponButton = false;

int main(int argc, char* argv[])
{
	//argv 0 is program name as usual
	//argv 1 is the path to a settings file.
	//If none is provided, the defaults will be used
	if(argc > 1)
	{
		//TODO: Read file and adjust settings accordingly
	}
	else
	{
		settings = defaultSettings;
	}
	
	mode = MODE_EPEE;
	
	while(1)
	{
		currentlyPressingWeaponButton = GetDigitalPinState(IPIN_WEAPON_BUTTON);
		//switch modes if necessary
		if(previouslyPressingWeaponButton && !currentlyPressingWeaponButton)
		{
			SetMode((++mode) % 3);
		}
		
		SetFaultLights();
		//run the current mode
		switch(mode)
		{
			case MODE_EPEE:
				EpeeMode();
				break;
			default:
				break;
		}
		previouslyPressingWeaponButton = currentlyPressingWeaponButton;
	}
}

void SetMode(Mode newMode)
{
	//Reset everything that we can output
	SetPinState(OPIN_LEFT_A, 0);
	SetPinState(OPIN_RIGHT_A, 0);
	SetPinState(OPIN_LEFT_FAULT, 0);
	SetPinState(OPIN_RIGHT_FAULT, 0);
	SetPinState(OPIN_LEFT_TARGET, 0);
	SetPinState(OPIN_RIGHT_TARGET, 0);
	SetPinState(OPIN_LEFT_OFF_TARGET, 0);
	SetPinState(OPIN_RIGHT_OFF_TARGET, 0);
	SetPinState(OPIN_BUZZER, 0);

	mode = newMode;
	left.active = false;
	right.active = false;
	buzzer.active = false;
	hasLockedOut = false;
	leftHit = false;
	rightHit = false;
	SetPinState(OPIN_WEAPON_EPEE + newMode, 1);
}

void SetFaultLights()
{
	SetPinState(OPIN_LEFT_FAULT, GetDigitalPinState(IPIN_LEFT_FAULT));
	SetPinState(OPIN_RIGHT_FAULT, GetDigitalPinState(IPIN_RIGHT_FAULT));
}

void SetLights()
{
	SetPinState(OPIN_LEFT_TARGET, leftHit);
	SetPinState(OPIN_RIGHT_TARGET, rightHit);
	SetPinState(OPIN_LEFT_OFF_TARGET, leftOffTarget);
	SetPinState(OPIN_RIGHT_OFF_TARGET, rightOffTarget);
}

void HandleLockout()
{
	//Set the lights again, just in case
	SetLights();
	
	//Set the buzzer and wait
	// TODO: check whether the buzzing and lights should start at when a hit is received
	SetPinState(OPIN_BUZZER, 1);
	Wait(settings.buzzerTime);
	SetPinState(OPIN_BUZZER, 0);
	Wait(settings.lightTime - settings.buzzerTime);
	
	//Reset state
	leftHit = false;
	rightHit = false;
	leftOffTarget = false;
	rightOffTarget = false;
	hasLockedOut = false;
	
	//Reset lights
	SetLights();
}

void WaitShortTime(FencingClock waitTime)
{
	FencingClock start = PlatformClock();
	while(PlatformClock() - start < waitTime)
	{
		//do nothing
	}
	
}

bool EpeeCheckWeapon(bool leftFencer)
{
	Pins fencerA;
	Pins fencerB;
	Pins opponentC;
	Pins floor = IPIN_FLOOR;
	if(leftFencer)
	{
		fencerA = OPIN_LEFT_A;
		fencerB = IPIN_LEFT_B;
		opponentC = IPIN_RIGHT_C;
	}
	else
	{
		fencerA = OPIN_RIGHT_A;
		fencerB = IPIN_RIGHT_B;
		opponentC = IPIN_LEFT_C;
	}
	
	SetPinState(fencerA, 1); //should be redefined to whatever "high" is. Right now 1 is just meant to represent "on" assuming a digital pin.
	WaitShortTime(settings.wirePropagationTime);

	bool floorState = GetDigitalPinState(floor);
	bool fencerBState = GetDigitalPinState(fencerB);
	bool opponentCState = GetDigitalPinState(opponentC);

	bool status = (!floorState && fencerBState && !opponentCState);
	
	SetPinState(fencerA, 0); //should be redefined to whatever "low" is. See above.
	
	//Wait a little bit of time just in case
	WaitShortTime(settings.wirePropagationTime);
	
	return status;
}

void EpeeMode()
{
	FencingClock currentTime = PlatformClock();
	
	if((leftHit && (currentTime - left.timeStarted > settings.epeeLockoutTime)) ||
		(rightHit && (currentTime - right.timeStarted > settings.epeeLockoutTime)))
	{
		HandleLockout();
	}

	if(EpeeCheckWeapon(true))
	{
		if(left.active)
		{
			if(!leftHit && currentTime - left.timeStarted > settings.epeeDebounceTime)
			{
				leftHit = true;
				SetLights();
			}
		}
		else
		{
			left = (FencingTimeTracker){ currentTime, true };
		}
	}
	else
	{
		left.active = false;
	}
	
	if(EpeeCheckWeapon(false))
	{
		if(right.active)
		{
			if(!rightHit && currentTime - right.timeStarted > settings.epeeDebounceTime)
			{
				rightHit = true;
				SetLights();
			}
		}
		else
		{
			right = (FencingTimeTracker){ currentTime, true };
		}
	}
	else
	{
		right.active = false;
	}
}
