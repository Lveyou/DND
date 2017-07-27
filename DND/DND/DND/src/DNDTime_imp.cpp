#include "DNDTime_imp.h"
#include <ctime>

namespace DND
{

	Time_imp::Time_imp() : 
		_start(static_cast<UInt64>(time(0))),
		_loopStart(0),
		_count(0),
		_delta(0),
		_fps(0)
	{
		QueryPerformanceFrequency(&_freq);
		/*QueryPerformanceCounter(&_current);
		_last = _current;*/
	}

	String Time_imp::GetHMSString()
	{
		time_t tt = time(0);
		tm t;
		localtime_s(&t, &tt);


		return String::Format(10, L"%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
	}

	UInt32 Time_imp::GetFPS()
	{
		return _fps;
	}

	void Time_imp::_init_loop_start()
	{
		_loopStart = time(0);
		QueryPerformanceCounter(&_last);
		
	}

	void Time_imp::_update()
	{
		QueryPerformanceCounter(&_current);
		_delta = (double)(_current.QuadPart - _last.QuadPart) / (double)_freq.QuadPart;
		
		//
		/*if(_delta < 0.0001)
		_delta = 0.0001;*/

		static double elapsed = 0;
		static UInt32 count = 0;
		elapsed += _delta;
		count++;
		if(elapsed >= 1.0)
		{
			_fps = static_cast<UInt32>((double)count / elapsed);
			elapsed = 0;
			count = 0;
		}

		_count += _delta;
	}

	void Time_imp::_set_last()
	{
		_last = _current;
	}

	double Time_imp::GetDelta()
	{
		return _delta;
	}

}

