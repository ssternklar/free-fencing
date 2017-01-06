/**
 * WindowsTestingHardwareLayer.c
 *
 * Created on: 1/5/2016
 *  Author(s): Samuel Sternklar
 *
 * This file is meant to be a test of the FencingHardwareLayer implementation,
 * Allowing us to test things in a simple console window while we don't have
 * access to hardware.
 */

#include "FencingHardwareLayer.h"

#ifdef WINDOWS

#include <Windows.h>
#include <stdio.h>

unsigned int input;

void InitializeIO()
{
	input = 0;
}

bool GetDigitalPinState(Pins pin)
{
	return (input & (1 << pin)) > 0;
}

void SetPinState(Pins outputPin, int state)
{
	input ^= (-state ^ input) & (1 << outputPin);
	if (outputPin == OPIN_LEFT_A)
	{
		input ^= (-state ^ input) & (1 << IPIN_LEFT_B);
	}
	else if (outputPin == OPIN_LEFT_TARGET && state == 1)
	{
		puts("Left fencer hit");
	}
	else if (outputPin == OPIN_RIGHT_TARGET && state == 1)
	{
		puts("Right fencer hit");
	}
}

void Wait(FencingClock timeToWait)
{
	DWORD sleepTime = (int)(timeToWait / 1000);
	Sleep(sleepTime);
}

FencingClock PlatformClock()
{
	FencingClock ret;
	GetSystemTimePreciseAsFileTime(&ret);
	ret /= 10;
	return ret;
}

#endif