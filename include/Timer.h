#ifndef TIMER_H
#define TIMER_H

#include "Time_stamp.h"
#include <functional>
#include <iostream>

/*
 * 定时事件类，包含到时中间 和 时间间隔 以及到时后要调用的回调函数
 * 每一个对象对应于一个定时任务
 */

namespace tiny
{

class Timer
{
public:
	typedef std::function<void(void)> Timer_callback;
	Timer(Time_stamp at, Time_stamp it);
	void set_callback(Timer_callback);
	bool operator<(const Timer &t) const
	{
		return _arrival_time > t._arrival_time;
	}

	bool operator>(const Timer &t) const
	{
		return _arrival_time < t._arrival_time;
	}

	bool operator==(const Timer &t) const
	{
		return _arrival_time == t._arrival_time;
	}

	bool operator<=(const Timer &t) const
	{
		return _arrival_time >= t._arrival_time;
	}


	bool operator>=(const Timer &t) const
	{
		return _arrival_time <= t._arrival_time;
	}

	void handler()
	{
		if(_callback != NULL)
			_callback();
	}

	bool interval()
	{
		return _interval;
	}

	void reset(Time_stamp, Time_stamp);  // 重设时钟

	Time_stamp get_arrival_time()
	{
		return _arrival_time;
	}

	Time_stamp get_interval_time()
	{
		return _interval_time;
	}

	void debug()
	{
		std::cout << "Timer debug: " << _arrival_time.get_microsecond() << std::endl;
	}
	
private:
	Time_stamp _arrival_time;  // 到时时间，采用日历时间，即从1970年1月1日到到时时的微秒数
	Time_stamp _interval_time; // 间隔时间
	bool _interval;            // 是否设置了间隔时间
	Timer_callback _callback;  // 时钟到时后的回调函数
};

}

#endif
