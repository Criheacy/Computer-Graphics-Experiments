#include "Animator.h"

template<typename T>
Animator<T>::Animator()
{
	startTime = 0;
	duration = 0;
	enable = false;
	defaultValue = fromValue;
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
	return fromValue * (1.0 - ratio) + toValue * ratio;
}

template<typename T>
int Animator<T>::NowTime()
{
	return glutGet(GLUT_ELAPSED_TIME);
}