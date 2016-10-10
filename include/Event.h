#ifndef EVENT_H
#define EVENT_H

namespace tiny
{
/*
 * 定义了Event_handler事件以及与Epoll 和 Poll 事件之间的转换函数
 */

typedef short Event;

enum EVENT{EVENTIN=1, EVENTOUT=2, EVENTERR=4, EVENTHUP=8, EVENTRDHUP=16,
		EVENTPRI};

short Event_to_poll(Event ev);

short Event_to_epoll(Event ev);

Event Epoll_to_event(short ev);

short Poll_to_event(short ev);

}

#endif
