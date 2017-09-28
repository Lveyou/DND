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

namespace DND
{
	class Time_imp : public Time
	{
		friend class Game;
		friend class GameServer;
	public:
		//example: "14:47:39"
		virtual String GetHMSString() override;
		virtual UINT32 GetFPS() override;
		virtual UINT32 GetRealFPS() override;
		virtual double GetDelta() override;
		virtual double GetRealDelta() override;
		virtual double GetCurrent() override;
		virtual void SetFPS(UINT32 fps) override;
	private:
		Time_imp();
		
		double _real_delta;			//(s)真实间隔时间
		UINT32 _real_fps;			//真实帧数
		double _delta;				//(s)一帧应该运行的时间
		UINT32 _fps;				//设置的帧数
		double _except_render;		//估算的绘图时间（CPU->GPU->显示器）
	
		//下面的变量 主要计算两帧之间时间差
		LARGE_INTEGER _start;//程序开始
		LARGE_INTEGER _loop_start;//程序开始
		LARGE_INTEGER _freq;//cpu时钟频率，一秒多少次时钟周期
		LARGE_INTEGER _last;//前一个累计时钟周期
		LARGE_INTEGER _current;//当前的累计时钟周期
		void _update_current();
		void _set_last();
		double _get_cl_delta();
	};
}

#endif
