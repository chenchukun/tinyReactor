#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <functional>
#include <string>
#include <map>
#include <atomic>
#include <mutex>
#include "Internet_addr.h"
#include "Accept.h"
#include "Reactor.h"
#include "Tcp_connection.h"

namespace tiny
{
/* TCP Server类，用于建立一个TCP监听链接
 * 并接受客户端的链接
 */ 

class Tcp_server
{
public:
	typedef std::function<void(int, Internet_addr&)> Newconnection_callback;
	typedef std::function<void(Tcp_connection_ptr)> Connection_callback;
	typedef std::function<void(Tcp_connection_ptr)> Message_callback;
	typedef std::function<void(Tcp_connection_ptr)> Error_callback;
	typedef std::function<void(Tcp_connection_ptr)> Shutdown_callback;
	typedef std::function<void(int)> Remove_callback;
	typedef std::function<void(void)> Callback;

	Tcp_server(Reactor *reactor, const Internet_addr& addr, const std::string &name);
	Tcp_server(Reactor *reactor, const Internet_addr& addr);
	~Tcp_server(){}
	void start();
	void set_connection_callback(Connection_callback cb);
	void set_message_callback(Message_callback cb);
	void set_error_callback(Error_callback cb);
	void set_shutdown_callback(Shutdown_callback cb);

private:
	Reactor *_reactor;
	const std::string _name;
	Accept _accept;
	std::map<int, Tcp_connection_ptr> _connection_map;
	std::mutex _mutex;

	Connection_callback _c_callback;
	Message_callback _m_callback;
	Error_callback _e_callback;
	Shutdown_callback _s_callback;

	bool _set_e = false;
	bool _set_m = false;
	bool _set_s = false;

//	int _num;
	std::atomic_int _num;

	void newconnection_callback(int fd, const Internet_addr &addr);
	void remove_connection(int key);
};

}

#endif
