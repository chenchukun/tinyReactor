#include "Tcp_connection.h"
#include <assert.h>
#include <iostream>

namespace tiny
{

Tcp_connection::Tcp_connection(Reactor *r, int fd, int id, const Internet_addr &la, 
		const Internet_addr &pa)
	: _reactor(r), _socket(fd), _localaddr(la), _peeraddr(pa), _event_handler(fd),
	_m_callback(NULL), _e_callback(NULL), _s_callback(NULL), _r_callback(NULL), _num_id(id)
{
	_socket.set_nonblock(true);
	_event_handler.set_read_event(true);
	_event_handler.set_read_callback(std::bind(&Tcp_connection::handler_read, this));
	_event_handler.set_err_callback(std::bind(&Tcp_connection::handler_error, this));
	_reactor->add_event(&_event_handler);	
	_write_buffer.set_write_callback(std::bind(&Tcp_connection::buffer_no_empty, this));
//	std::cout << "Tcp_connection debug: Tcp_connection()" << std::endl;
}

// 主动关闭
void Tcp_connection::shutdown()
{
	_reactor->del_event(&_event_handler);
//	_socket.shutdown();
	// 调用回调从Tcp_server中删除连接
	_r_callback(_num_id);
}

const Internet_addr* Tcp_connection::get_localaddr() const
{
	return &_localaddr;
}

const Internet_addr* Tcp_connection::get_peeraddr() const
{
	return &_peeraddr;
}

void Tcp_connection::set_message_callback(Message_callback cb)
//void Tcp_connection::set_message_callback(Callback cb)
{
	if(cb != NULL)
	{
//		std::cout << "Tcp_connection debug: set_read_event(true)" << std::endl;
		_event_handler.set_read_event(true);
		_reactor->update(_event_handler.get_fd());
	}
	_m_callback = cb;
}

//void Tcp_connection::set_error_callback(Callback cb)
void Tcp_connection::set_error_callback(Error_callback cb)
{
	if(cb != NULL)
	{
		_event_handler.set_err_event(true);
		_reactor->update(_event_handler.get_fd());
	}
	_e_callback = cb;
}

//void Tcp_connection::set_shutdown_callback(Callback cb)
void Tcp_connection::set_shutdown_callback(Error_callback cb)
{
	_s_callback = cb;
}

void Tcp_connection::set_remove_connection_callback(Remove_callback cb)
{
	_r_callback = cb;
}

void Tcp_connection::handler_read()
{
//	std::cout << "Tcp_connection debug: handler_read()" << std::endl;
	char buf[2048];
	int n = _socket.recv(buf, 2048);
	if(n <= 0)  // 什么时候返回0，什么时候返回-1？
	{
//		std::cout << "Tcp_connection debug: read return 0" << std::endl;
		// 对方关闭连接
		handler_shutdown();
		return;
	}
	// 写入缓冲区
	_read_buffer.write(buf, n);
	
	if(_m_callback != NULL)
	{
//		_m_callback(Tcp_connection_ptr(this));
		_m_callback(shared_from_this());
	}
}

void Tcp_connection::handler_shutdown()
{
//	std::cout << "Tcp_connection debug: handler_shutdown()" << std::endl;
	if(_s_callback != NULL)
	{
//		std::cout << "Tcp_connection debug: call _s_callback" << std::endl;
//		_s_callback(Tcp_connection_ptr(this));
		_s_callback(shared_from_this());
	}
//	std::cout << "Tcp_connection debug: del_event start" << std::endl;
	_reactor->del_event(&_event_handler);
//	_socket.shutdown();
//	std::cout << "Tcp_connection debug: del_event end" << std::endl;
	// 调用回调从Tcp_server中删除连接
	_r_callback(_num_id);
}

void Tcp_connection::handler_error()
{
	if(_e_callback != NULL)
	{
//		_e_callback(Tcp_connection_ptr(this));
		_e_callback(shared_from_this());
	}
	_reactor->del_event(&_event_handler);
//	_socket.shutdown();
	// 调用回调从Tcp_server中删除连接
	_r_callback(_num_id);
}

void Tcp_connection::buffer_no_empty()
{
//	std::cout << "Tcp_connection debug: buffer_no_empty() " << _write_buffer.size() << std::endl;
	_event_handler.set_write_callback(std::bind(&Tcp_connection::handler_write, this));	
	_event_handler.set_write_event(true);
	_reactor->update(_event_handler.get_fd());
}

void Tcp_connection::handler_write()
{
//	std::cout << "Tcp_connection debug: handler_write()" << std::endl;
	if(_write_buffer.size() == 0)
	{
//		std::cout << "Tcp_connection debug: handler_write() size()==0" << std::endl;
		_event_handler.set_write_event(false);
		_reactor->update(_event_handler.get_fd());
		return;
	}
	int len = _socket.send(_write_buffer.head(), _write_buffer.size());
//	std::cout << "Tcp_connection debug: send return: " << len  << std::endl;
	assert(len != -1);
	_write_buffer.discard_nbyte(len);
}

}
