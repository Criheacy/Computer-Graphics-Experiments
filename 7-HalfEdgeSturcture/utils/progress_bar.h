#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <sys/time.h>
#include <cmath>
#include <string>
#include <iostream>

class ProgressBar {
public:
	explicit ProgressBar(int maxValue, int value = 0);
	int GetProgressValue() const;
	double GetProgressRate() const;
	void SetProgressValue(int value);
	void UpdateProgressValue(int deltaValue);
	void SetProgressTitle(std::string title);

	void SetInterceptInterval(unsigned long long interceptValue);

	/// Print current progress bar to terminal command line.
	/// \param force
	/// If force=true, logging function will ignore the intercept interval
	/// and print process bar forcefully. otherwise the log will print only
	/// if interval since last printing is larger than intercept value.
	/// \return Returns whether the log prints successfully. Always returns
	/// true when force=true.
	bool Log(bool force = false);
	std::string LogString();

private:
	const int PROGRESS_BAR_LENGTH = 50;
	const int PROGRESS_BAR_BLOCKS_TYPE = 8;
	const std::string PROGRESS_BAR_BLOCKS[9] = {" ", "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█"};

	const unsigned long long DEFAULT_INTERCEPT_VALUE = 200;

	static unsigned long long GetMillisecond();
	bool InterceptCheck() const;

	std::string mTitle;
	bool isFirstLog;
	int mCurrentValue;
	int mMaxValue;

	unsigned long long interceptValue;
	unsigned long long lastLoggingTime;

};

#endif  //PROGRESS_BAR_H
