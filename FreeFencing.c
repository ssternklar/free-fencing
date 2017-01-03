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
bool leftHit;
bool rightHit;
bool hasLockedOut;
Mode mode = MODE_EPEE;
int currentInput = 0;
int previousInput = 0;

int main(int argc, char* argv[])
{
	//argv 0 is program name as usual
	//argv 1 is the path to a settings file.
	//If none is provided, the defaults will be used
	if(argc > 1)
	{
		//TODO: Read file and adjust settings accordingly
	}
	
	mode = MODE_EPEE;
	
	while(1)
	{
		currentInput = GetDigitalPinStates();
		//switch modes if necessary
		if(previousInput & IPIN_WEAPON_SWITCH && !(currentInput & IPIN_WEAPON_SWITCH))
		{
			SwitchMode((++mode) % 3);
		}
		
		//run the current mode
		switch(mode)
		{
			case MODE_EPEE:
				EpeeMode();
				break;
			default:
				break;
		}
		previousInput = currentInput;
	}
}

void SetMode(Mode newMode)
{
	//Reset everything that we can output
	SetDigitalPinStates(0);
	mode = newMode;
	FencingTracking newTracking;
	tracking = newTracking;
	SetPinState(OPIN_WEAPON_EPEE + newMode, 1);
}

void HandleLockout()
{
	//TODO: Implement lockout
}

bool EpeeCheckWeapon(int input, bool leftFencer)
{
	Pins fencerA;
	Pins fencerB;
	Pins opponentC;
	Pins floor = IPIN_FLOOR;
	if(leftFencer)
	{
		fencerA = 1 << IPIN_LEFT_A
		fencerB = 1 << IPIN_LEFT_B;
		opponentC = 1 << IPIN_RIGHT_C;
	}
	else
	{
		fencerA = 1 << IPIN_RIGHT_A
		fencerB = 1 << IPIN_RIGHT_B;
		opponentC = 1 << IPIN_LEFT_C;
	}
	return (
		(input & floor) == 0 && //The floor is not being hit
		(input & fencerA) > 0 && // this fencer's A is active
		(input & fencerB) > 0 && // this fencer's B is active
		(~input & opponentC) == 0 // this fencer's C is NOT active
	);
}

void EpeeMode()
{
	FencingClock currentTime = PlatformClock();
	
	if(leftHit && currentTime - left.timeStarted > settings.epeeLockoutTime ||
		rightHit && currentTIme - right.timeStarted > settings.epeeLockoutTime)
		hasLockedOut = true;

	if(!hasLockedOut)
	{
		if(EpeeCheckWeapon(currentInput, true))
		{
			if(left.active)
			{
				if(currentTime - left.timeStarted > settings.epeeDebounceTime)
				{
					leftHit = true;
				}
			}
			else
			{
				left = { currentTime, true };
			}
		}
		else
		{
			left.active = false;
		}
		
		if(EpeeCheckWeapon(currentInput, false))
		{
			if(right.active)
			{
				if(currentTime - right.timeStarted > settings.epeeDebounceTime)
				{
					rightHit = true;
				}
			}
			else
			{
				right = { currentTime, true };
			}
		}
		else
		{
			right.active = false;
		}
	}
}