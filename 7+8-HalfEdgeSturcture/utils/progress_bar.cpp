#include "progress_bar.h"

#include <utility>

ProgressBar::ProgressBar(int maxValue, int value) {
	mMaxValue = maxValue;
	mCurrentValue = value;
	interceptValue = DEFAULT_INTERCEPT_VALUE;
	lastLoggingTime = 0;
	isFirstLog = true;
}

int ProgressBar::GetProgressValue() const {
	return mCurrentValue;
}

double ProgressBar::GetProgressRate() const {
	return static_cast<double>(mCurrentValue) / mMaxValue;
}

void ProgressBar::SetProgressValue(int value) {
	mCurrentValue = value;
}

void ProgressBar::UpdateProgressValue(int deltaValue) {
	mCurrentValue += deltaValue;
}

void ProgressBar::SetInterceptInterval(unsigned long long int interceptValue) {
	this->interceptValue = interceptValue;
}

void ProgressBar::SetProgressTitle(std::string title) {
	this->mTitle = std::move(title);
}

bool ProgressBar::Log(bool force) {
	bool logPermission = false;
	if (!force) {
		logPermission = InterceptCheck() || (mCurrentValue == mMaxValue);
	}
	if (force || logPermission) {
		lastLoggingTime = GetMillisecond();

		if (isFirstLog) {
			printf("START %s:\n", mTitle.c_str());
			isFirstLog = false;
		}

		if (mCurrentValue == mMaxValue) {
			printf("[%s] (%d/%d) - Complete\n", LogString().c_str(),
			       mCurrentValue, mMaxValue);
		} else {
			printf("[%s] (%d/%d) - %.1lf%%\r", LogString().c_str(),
			       mCurrentValue, mMaxValue, GetProgressRate() * 100);
		}
		std::cout << std::flush;
		return true;
	}
	return false;
}

std::string ProgressBar::LogString() {
	std::string res;
	double progressRate = GetProgressRate();
	int progressLength = floor(progressRate * PROGRESS_BAR_LENGTH * PROGRESS_BAR_BLOCKS_TYPE);
	int currentBlockIndex;
	for (int i = 0; i < PROGRESS_BAR_LENGTH * PROGRESS_BAR_BLOCKS_TYPE; i += PROGRESS_BAR_BLOCKS_TYPE) {
		currentBlockIndex = progressLength - i;

		// clamp value in [0, PROGRESS_BAR_BLOCKS_TYPE]
		if (currentBlockIndex < 0) {
			currentBlockIndex = 0;
		}
		if (currentBlockIndex > PROGRESS_BAR_BLOCKS_TYPE) {
			currentBlockIndex = PROGRESS_BAR_BLOCKS_TYPE;
		}

		res += PROGRESS_BAR_BLOCKS[currentBlockIndex];
	}
	return res;
}

bool ProgressBar::InterceptCheck() const {
//	printf("%llu-%llu=%llu <=> %llu\n", GetMillisecond(), lastLoggingTime,
//	       GetMillisecond() - lastLoggingTime, interceptValue);
	return GetMillisecond() - lastLoggingTime >= interceptValue;
}

unsigned long long ProgressBar::GetMillisecond() {
	struct timeval _timeval {};
	gettimeofday(&_timeval, nullptr);

	return (unsigned long long)(_timeval.tv_sec) * 1000 +
		(unsigned long long)(_timeval.tv_usec) / 1000;
}
