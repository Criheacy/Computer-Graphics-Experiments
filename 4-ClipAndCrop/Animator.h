#pragma once

#include <glut.h>

template<typename T>
class Animator
{
public:	
	Animator();
	void SetValueRange(T from, T to);
	void SetDuration(int millisecond);
	void StartAnimation();
	void StopAnimation();
	T GetValue();

protected:
	int NowTime();
	bool enable;
	T fromValue;
	T toValue;
	T defaultValue;
	int startTime;
	int duration;
};