#include "GlutTimer.h"
#include <cstdlib>
#include <GL/glut.h>
#include <windows.h>

extern CRITICAL_SECTION critical_sec;

void (*GlutTimer::func)() = 0;
int GlutTimer::interval_ms = 33;
bool GlutTimer::isOn = false;

GlutTimer::GlutTimer(void (*func_)(), double interval_s_)
{
	func = func_;
	interval_ms = (int)(interval_s_ * 1000.0);
	isOn = false;
}

bool GlutTimer::start()
{
	if (isOn) {
		return false;
	}
	isOn = true;
	glutTimerFunc(interval_ms, realTimerFunc, 0);
	return true;
}

bool GlutTimer::stop()
{
	if (!isOn) {
		return false;
	}
	isOn = false;
	return true;
}

void GlutTimer::realTimerFunc(int value)
{
	EnterCriticalSection(&critical_sec);
	func();
	if (isOn) {
		glutTimerFunc(interval_ms, realTimerFunc, 0);
	}
	LeaveCriticalSection(&critical_sec);
}
