#ifndef _Timer_H_
#define _Timer_H_

#include <string>

class Timer {
private:
	double start_time;

public:
	Timer();
	void Touch();
	double Ago();
	std::string strAgo();
};

#endif
