#include "DNDTime_imp.h"
#include <ctime>

namespace DND
{

	Time_imp::Time_imp() : 
		_start(static_cast<UINT64>(time(0))),
		_loopStart(0),
		_real_delta(0),
		_real_fps(0),
		_delta(1.0/60),
		_fps(60),
		_except_render(0)
	{
		QueryPerformanceFrequency(&_freq);
	}

	String Time_imp::GetHMSString()
	{
		time_t tt = time(0);
		tm t;
		localtime_s(&t, &tt);


		return String::Format(10, L"%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
	}

	UINT32 Time_imp::GetFPS()
	{
		return _fps;
	}

	double Time_imp::GetDelta()
	{
		return _real_delta;
	}

	void Time_imp::SetFPS(UINT32 fps)
	{
		_fps = fps;
		_delta = 1.0 / _fps;
	}

	void Time_imp::_set_last()
	{
		_last = _current;
	}

	void Time_imp::_update_current()
	{
		QueryPerformanceCounter(&_current);
	}

	double Time_imp::_get_cl_delta()
	{
		return (double)(_current.QuadPart - _last.QuadPart) / (double)_freq.QuadPart;
	}

	UINT32 Time_imp::GetRealFPS()
	{
		return _real_fps;
	}



}

