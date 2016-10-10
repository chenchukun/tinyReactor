#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <functional>
#include <stdint.h>
#include <iostream>
#include "Event.h"

/*
 * 事件处理类，每个对象对应一个文件描述符，包含该文件描述符要监听的事件，
 * 已经发生的事件，已经对应的处理方法。
 */ 
namespace tiny
{

class Event_handler
{
public:
	typedef std::function<void(void)> Callback;

	Event_handler(int fd):_fd(fd), _event(0), _revent(0),
		_in_callback(NULL), _out_callback(NULL), _err_callback(NULL)
	{
	}

	Event_handler(const Event_handler&) = delete;
	Event_handler& operator=(const Event_handler&) = delete;
	~Event_handler(){}

	void set_read_event(bool);    // 设置是否检测读事件
	void set_write_event(bool);   // 设置是否检测写事件
	void set_err_event(bool);     // 设置是否检测错误事件

	void set_read_callback(Callback cb)
	{
		_in_callback = cb;
	}
	void set_write_callback(Callback cb)
	{
		_out_callback = cb;
	}
	void set_err_callback(Callback cb)
	{
		_err_callback = cb;
	}

	Event get_event()  // 获取需要检测的事件
	{
		return _event;
	}

	Event get_revent()  // 获取发生的事件
	{
		return _revent;
	}
	// 设置 发生的事件为给定值
	void set_revent(Event ev)  // 设置发生的事件为给定值
	{
		_revent = ev;
	}

	void set_event(Event ev)  // 设置要检测的事件为给定值
	{
		_event = ev;
	}
	
	void handler();  // 处理发生的事件，当有时间发生时可以调用该函数来统一处理

	int get_fd()  // 获取文件描述符
	{
		return _fd;
	}


private:
	void set_event(EVENT ev, bool b);

	int _fd;
	Event _event;    // 监听的事件
	Event _revent;   // 发生的事件

	Callback _in_callback;
	Callback _out_callback;
	Callback _err_callback;
};

}

#endif
