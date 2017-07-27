//////////////////////////////////////////////////////////////////////////
//name:		DNDTime_imp
//author:	Lveyou
//data:		17-07-27

//other:
//17-07-27: 时间实现 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_TIME_IMP_H_
#define _DND_TIME_IMP_H_


#include "DNDTime.h"
#include "DNDUser.h"
#include <windows.h>
#include <ctime>

namespace DND
{
	class Time_imp : public Time
	{
		friend class Game;
	public:
		//example: "14:47:39"
		virtual String GetHMSString() override;
		virtual UInt32 GetFPS() override;
		virtual double GetDelta() override;
	private:
		Time_imp();
		void _init_loop_start();
		void _update();
		void _set_last();
		UInt64 _start;
		UInt64 _loopStart;
		double _count;//(s), Frame func run calc count.
		double _delta;//(s), not Equal to 1/FPS
		UInt32 _fps;//  frame func calc times in one second.
		//下面的变量 主要计算两帧之间时间差
		LARGE_INTEGER _freq;//cpu时钟频率，一秒多少次时钟周期
		LARGE_INTEGER _last;//上一帧累计时钟周期
		LARGE_INTEGER _current;//当前的累计时钟周期
	};
}

#endif
