#ifndef TIMER_H
#define TIMER_H

#include <string>

class Timer
{
public:
	Timer();
	
	// Reset
	void Touch();
	
	// This returns the difference between now and touch
	double Ago() const;

	std::string StrAgo() const;
	
private:
	// The time we were created, or touched
	double m_iStart;
};

#endif
