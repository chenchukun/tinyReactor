#include "Event_handler.h"
#include <iostream>

// 生命期由创建者自己管理

namespace tiny
{

void Event_handler::set_event(EVENT ev, bool b)
{
//	std::cout << "Event_handler debug: _event: " << _event << std::endl;
	if(b)
	{
		_event |= ev;
	}
	else
	{
		_event &= ~ev;
	}
//	std::cout << "Event_handler debug: _event: " << _event << std::endl;
}

void Event_handler::set_read_event(bool b)
{
//	std::cout << "Event_handler debug: set_read_event" << std::endl;
	set_event(EVENTIN, b);
}

void Event_handler::set_write_event(bool b)
{
//	std::cout << "Event_handler debug: set_write_event" << std::endl;
	set_event(EVENTOUT, b);
}

void Event_handler::set_err_event(bool b)
{
//	std::cout << "Event_handler debug: set_err_event" << std::endl;
	set_event(EVENTERR, b);
}

void Event_handler::handler()
{
//	std::cout << "handler" << std::endl;
	if(_revent & (EVENTIN | EVENTRDHUP | EVENTHUP | EVENTPRI))
	{
//		std::cout << "in_callback()" << std::endl;
		if(_in_callback != NULL)
			_in_callback();
	}
	if(_revent & EVENTOUT)
	{
//		std::cout << "out_callback()" << std::endl;
		if(_out_callback != NULL)
			_out_callback();
	}
	if(_revent & EVENTERR)
	{
//		std::cout << "err_callback()" << std::endl;
		if(_err_callback != NULL)
			_err_callback();
	}
}

}
