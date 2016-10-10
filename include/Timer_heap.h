#ifndef TIMER_HEAP
#define TIMER_HEAP

#include <queue>
#include "Event_handler.h"
#include "Time_stamp.h"
#include "Timer.h"
#include <functional>

/*
 * 时间堆类，使用Timerfd来时间将定时任务与一个事件循环结合在一起。
 * 采用STL中的优先队列来管理定时器。
 */

namespace tiny
{

class Reactor;

class Timer_heap
{
public:
	typedef std::function<void(void)> Timer_callback;
	Timer_heap();
	void add_timer(Time_stamp, Time_stamp, Timer_callback);
	Event_handler* get_event_handler();


private:
	void reset_timer(Timer&);
	void start_timer();
	void read_timer();
	void handler();
	int create_timer();

	int _timer_fd;  // timerfd_create创建的文件描述符
	std::priority_queue<Timer> _timer_queue; // Timer优先队列
	Event_handler _timer_handler;  // 定时器时间处理类，用于添加到Reactor中
};

}

#endif
