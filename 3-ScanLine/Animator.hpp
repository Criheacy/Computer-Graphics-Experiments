#pragma once

#include "Settings.h"

template<typename T>
class Animator
{
public:
	Animator(T defaultValue);
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

template<typename T>
Animator<T>::Animator(T defaultValue)
{
	startTime = 0;
	duration = 0;
	enable = false;
	this->defaultValue = defaultValue;
}

template<typename T>
void Animator<T>::SetValueRange(T from, T to)
{
	fromValue = from;
	toValue = to;
	defaultValue = fromValue;
}

template<typename T>
void Animator<T>::SetDuration(int millisecond)
{
	duration = millisecond;
}

template<typename T>
void Animator<T>::StartAnimation()
{
	enable = true;
	startTime = NowTime();
}

template<typename T>
void Animator<T>::StopAnimation()
{
	enable = false;
	defaultValue = toValue;
}

template<typename T>
T Animator<T>::GetValue()
{
	double ratio = (static_cast<double>(NowTime() - startTime)) / duration;
	if (ratio > 1.0)
		StopAnimation();

	if (!enable) return defaultValue;
	return static_cast<T>(fromValue * (1.0 - ratio) + toValue * ratio);
}

template<typename T>
int Animator<T>::NowTime()
{
	return glutGet(GLUT_ELAPSED_TIME);
}