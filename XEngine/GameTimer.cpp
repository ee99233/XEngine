#include "GameTimer.h"



GameTimer::GameTimer()
	:msecondPerCount(0.0), mDeltaTime(-1.0),
	mPauseTime(0.0), mprevtime(0.0),mCurrtime(0.0f)
{
	INT64 queryf;
	 QueryPerformanceFrequency((LARGE_INTEGER*)(&queryf));
	msecondPerCount = 1.0 /(double) queryf;
	INT64 basetime;
	QueryPerformanceCounter((LARGE_INTEGER*)(&basetime));
	mbsetime = basetime;
}


GameTimer::~GameTimer()
{


}

float GameTimer::Deltatime() const
{
	return 0.0f;
}

float GameTimer::Totaltime() const
{
	return (float)((mCurrtime - mbsetime)*msecondPerCount);
}

void GameTimer::Reset()
{
}

void GameTimer::Start()
{
}

void GameTimer::Stop()
{
}

void GameTimer::Tick()
{

	QueryPerformanceCounter((LARGE_INTEGER*)(&mCurrtime));
	mDeltaTime = (mCurrtime - mprevtime)*msecondPerCount;
	mprevtime = mCurrtime;
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0f;
	}
	
}
