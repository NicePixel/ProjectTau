#include <chrono>
#include "timer.h"

using namespace std::chrono;

static time_point<steady_clock> savedclock;

void tau_timer_start(void)
{
	savedclock = steady_clock::now();
}

double tau_timer_stop(void)
{
	duration<double> span = duration_cast<duration<double>>(steady_clock::now() - savedclock);
	return span.count();
}
