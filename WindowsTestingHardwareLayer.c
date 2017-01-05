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

void InitializeIO()
{

}

bool GetDigitalPinState(Pins pin)
{

}

void SetPinState(Pins outputPin, int state)
{

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