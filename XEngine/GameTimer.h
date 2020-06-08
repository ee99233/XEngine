#pragma once
#include <windows.h>
class GameTimer
{
public:
	GameTimer();
	~GameTimer();
	float Deltatime() const;
	float Totaltime() const;
	void Reset();
	void Start();
	void Stop();
	void Tick();
private:
	double msecondPerCount;
	double mDeltaTime;
	INT64 mbsetime;
	INT64 mPauseTime;
	INT64 mstoptime;
	INT64 mprevtime;
	INT64 mCurrtime;
	bool mstop;
};

