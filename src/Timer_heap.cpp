#include "Timer_heap.h"
#include <sys/timerfd.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>

namespace tiny
{

// 创建定时器
int Timer_heap::create_timer()
{
//	int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
	int fd = timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC|TFD_NONBLOCK);
	assert(fd != -1);
	return fd;
}

Timer_heap::Timer_heap()
	:_timer_fd(create_timer()), _timer_handler(_timer_fd)
{
	// 初始化定时器的时间处理对象，设置监听读事件和读事件的回调函数
	_timer_handler.set_read_event(true);
	_timer_handler.set_read_callback(std::bind(&Timer_heap::handler, this));
}

// 添加定时任务
void Timer_heap::add_timer(Time_stamp at, Time_stamp it, Timer_callback cb)
{
	Timer timer(at, it);
	timer.set_callback(cb);
	_timer_queue.push(timer);
	start_timer();
}

// 当往时间堆里添加或删除定时事件后，需要调用该方法重新
// 选择最快到时的定时任务,并使用timerfd_settime来设置定时器
void Timer_heap::start_timer()
{
//	std::cout << "Timer_heap debug start_timer" << std::endl;
	if(_timer_queue.empty())
	{
		return;
	}

	Timer timer = _timer_queue.top();
	struct itimerspec itsp;
	itsp.it_value.tv_sec = timer.get_arrival_time().get_microsecond()/Time_stamp::MICRO_SECOND;
	itsp.it_value.tv_nsec = (timer.get_arrival_time().get_microsecond()
			%Time_stamp::MICRO_SECOND)*1000;

	itsp.it_interval.tv_sec = itsp.it_interval.tv_nsec = 0;
//	itsp.it_interval.tv_sec = timer.get_interval_time().get_microsecond()/Time_stamp::MICRO_SECOND;
//	itsp.it_interval.tv_nsec = (timer.get_interval_time().get_microsecond()
//			%Time_stamp::MICRO_SECOND)*1000;

	int ret = timerfd_settime(_timer_fd, TFD_TIMER_ABSTIME, &itsp, NULL);
	assert(ret != -1);
}


void Timer_heap::read_timer()
{
	char buf[8];
	int n = 8;
	int len = 0;
	while((len=read(_timer_fd, buf, n)) != n)
	{
		n -= len;
	}
}

// 系统时钟到时后调用
void Timer_heap::handler()
{
//	std::cout << "Timer_heap debug handler" << std::endl;
	read_timer();
	// 选择到时定时事件比当前时间小的任务，并执行其任务
	while(!_timer_queue.empty())
	{
		Timer timer = _timer_queue.top();
		Timer now_timer(Time_stamp::now(), 0);
		
		// Timer的比较是相反的
		if(timer >= now_timer)
		{
			_timer_queue.pop();
			timer.handler();
			reset_timer(timer);
		}
		else
		{
			break;
		}
	}
	start_timer();
}

// 定时任务到时候并处理完成后调用该函数，用于判断是否要重新设置定时任务
void Timer_heap::reset_timer(Timer &timer)
{
//	std::cout << "reset_timer" << std::endl;
	if(timer.interval())
	{
//		std::cout << "need reset_timer" << std::endl;
		timer.reset(timer.get_arrival_time()+timer.get_interval_time(), 
				timer.get_interval_time());
		_timer_queue.push(timer);
	}
}

// 获取事假处理类，用于添加到Reactor中
Event_handler* Timer_heap::get_event_handler()
{
	return &_timer_handler;
}

}
