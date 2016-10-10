#include "Tcp_server.h"
#include <iostream>
#include <assert.h>

namespace tiny
{

Tcp_server::Tcp_server(Reactor *reactor, const Internet_addr& addr, const std::string& name)
	:_reactor(reactor), _name(name), _accept(addr), _c_callback(NULL), 
	_m_callback(NULL), _num(0)
{
	_accept.set_newconnection_callback(std::bind(&Tcp_server::newconnection_callback, 
				this, std::placeholders::_1, std::placeholders::_2));		
}

Tcp_server::Tcp_server(Reactor *reactor, const Internet_addr& addr)
	:_reactor(reactor), _name(addr.to_string()), _accept(addr), _c_callback(NULL), 
	_m_callback(NULL), _num(0)
{
	_accept.set_newconnection_callback(std::bind(&Tcp_server::newconnection_callback, 
				this, std::placeholders::_1, std::placeholders::_2));		
}

void Tcp_server::start()
{
	_accept.listen();
	_reactor->add_event(_accept.get_event_handler());
}

void Tcp_server::set_connection_callback(Connection_callback cb)
{
	_c_callback = cb;
}

void Tcp_server::set_message_callback(Message_callback cb)
{
	_m_callback = cb;
	_set_m = true;
}

void Tcp_server::set_error_callback(Error_callback cb)
{
	_e_callback = cb;
	_set_e = true;
}

void Tcp_server::set_shutdown_callback(Shutdown_callback cb)
{
	_s_callback = cb;
	_set_s = true;
}

void Tcp_server::newconnection_callback(int fd, const Internet_addr &addr)
{
	Internet_addr localaddr;
	int ret = Socket(fd).get_addr(localaddr);
	assert(ret != -1);

	int key = _num;
	++_num;
	Tcp_connection_ptr connection_ptr = Tcp_connection_ptr(new Tcp_connection(_reactor, 
				fd, key, localaddr, addr));
	connection_ptr->set_weak_ptr(connection_ptr);

	if(_set_m)
	{
//		connection_ptr->set_message_callback(std::bind(_m_callback, connection_ptr));
		connection_ptr->set_message_callback(_m_callback);
		
	}
	if(_set_e)
	{
//		connection_ptr->set_error_callback(std::bind(_e_callback, connection_ptr));
		connection_ptr->set_error_callback(_e_callback);
	}
	if(_set_s)
	{
//		connection_ptr->set_shutdown_callback(std::bind(_s_callback, connection_ptr));
		connection_ptr->set_shutdown_callback(_s_callback);
	}

	connection_ptr->set_remove_connection_callback(std::bind(&Tcp_server::remove_connection, 
				this, std::placeholders::_1));


	{
		std::lock_guard<std::mutex> guard(_mutex);
		_connection_map[key] = connection_ptr;
	}

	// 调用回调
	if(_c_callback != NULL)
	{
		_c_callback(connection_ptr);
	}
//	std::cout << "Tcp_server debug: " << _connection_map.size() << std::endl;
	// debug
//	Socket(fd).send("Hello", 5);
}

void Tcp_server::remove_connection(int key)
{
	std::lock_guard<std::mutex> guard(_mutex);
	if( _connection_map.count(key) > 0 )
	{
//		std::cout << "Tcp_server debug: offline" << std::endl;
		_connection_map.erase(key);
	}
}

}


