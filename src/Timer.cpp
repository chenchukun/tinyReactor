#include "Timer.h"

namespace tiny
{

Timer::Timer(Time_stamp at, Time_stamp it)
	:_arrival_time(at), _interval_time(it), _interval(false), _callback(NULL)
{
	if(_interval_time != 0)
	{
		_interval = true;
	}
}

void Timer::reset(Time_stamp at, Time_stamp it)
{
	_arrival_time = at;
	_interval_time = it;
	_interval = it==0?false:true;
}

void Timer::set_callback(Timer_callback cb)
{
	_callback = cb;
}

}
