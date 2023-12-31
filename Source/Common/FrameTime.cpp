//*** FrameTime.cpp **

#include "FrameTime.h"
#include "Platform_Time.h"
#include <windows.h>
	
//*** Constructor ***

FrameTime::FrameTime():
	initialized_(false),
	previousFrameTime_(0),
	deltaTime_(0),
	frameCounter_(0),
	frameRateLock_( 0 )
	{
	}

void FrameTime::LockFrameRate( int fps )
	{
	frameRateLock_ = fps;
	}

//*** Update ***

float FrameTime::Update()
	{
	// If this is the first call to Update, we need to initialize previousFrameTime_
	if (!initialized_ && Platform::GetPlatform_Time())
		{
		previousFrameTime_=Platform::GetPlatform_Time()->GetTime();
		initialized_=true;
		}

	// Increase framecounter
	frameCounter_++;

	// Calculate deltatime
	float currentFrameTime=0;
	if (Platform::GetPlatform_Time())
		{
		currentFrameTime=Platform::GetPlatform_Time()->GetTime();
		}

	if( frameRateLock_ > 0 )
		{
		float delta = currentFrameTime - previousFrameTime_;
		int ms = (int)( 1000.0f / 60.0f - delta * 1000.0f );
		if( ms > 0 )
			{
			Sleep( ms );
			}
		//if (Platform::GetPlatform_Time())
		//	{
		//	currentFrameTime=Platform::GetPlatform_Time()->GetTime();
		//	}
		currentFrameTime += ms / 1000.0f;
		}
	
	deltaTime_=currentFrameTime-previousFrameTime_;
	previousFrameTime_=currentFrameTime;

	// Cap deltatime if it is too high (or things will jump like crazy on occasional long stalls)
	if (deltaTime_>1.0f/10.0f)
		{
		deltaTime_=1.0f/60.0f;
		}

	// Cap deltatime if it is negative or zero (this can actually happen due to floating point imprecisions)
	else if (deltaTime_<=0)
		{
		deltaTime_=1.0f/60.0f;
		}

	return deltaTime_;
	}


//*** GetDeltaTime ***

float FrameTime::GetDeltaTime()
	{
	return deltaTime_;
	}


//*** GetFrameCounter ***

int FrameTime::GetFrameCounter()
	{
	return frameCounter_;
	}
