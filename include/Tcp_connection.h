#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "Reactor.h"
#include "Socket.h"
#include "Internet_addr.h"
#include "Buffer.h"
#include "Event_handler.h"
#include <functional>
#include <memory>

namespace tiny
{

class Tcp_connection;
typedef std::shared_ptr<Tcp_connection> Tcp_connection_ptr;
typedef std::weak_ptr<Tcp_connection> Tcp_connection_weak_ptr;
class Tcp_connection
{
public:
	typedef std::function<void(Tcp_connection_ptr)> Message_callback;
	typedef std::function<void(Tcp_connection_ptr)> Error_callback;
	typedef std::function<void(Tcp_connection_ptr)> Shutdown_callback;
	typedef std::function<void(int)> Remove_callback;
	typedef std::function<void(void)> Callback;

	Tcp_connection(Reactor *r, int fd, int id, const Internet_addr &la, const Internet_addr &pa);

	~Tcp_connection()
	{
		_socket.shutdown();
//		std::cout << "Tcp_connection debug: ~Tcp_connection()" << std::endl;
	}
	void shutdown();
	const Internet_addr* get_localaddr() const;
	const Internet_addr* get_peeraddr() const;
	void set_message_callback(Message_callback cb);
//	void set_message_callback(Callback cb);
	void set_error_callback(Error_callback cb);
//	void set_error_callback(Callback cb);
	void set_shutdown_callback(Shutdown_callback cb);
//	void set_shutdown_callback(Callback cb);

	// Tcp_server类用，连接断开时调用回调
	void set_remove_connection_callback(Remove_callback cb);

	// 发送
	void send(const void *buf, int size)
	{
		_write_buffer.write(buf, size);
	}

	void send(std::string buf)
	{
		_write_buffer.write(buf);
	}

	// Tcp_server类调用, 使得Tcp_server类和Tcp_connection类贡献同一个shared_ptr<Tcp_connection>，
	// 不好的做法，有什么方法可以改进？
	void set_weak_ptr(Tcp_connection_ptr ptr)
	{
		_this_weak_ptr = ptr;
	}


	Buffer& read_buffer()
	{
		return _read_buffer;
	}

private:
	Tcp_connection_ptr shared_from_this()
	{
		return _this_weak_ptr.lock();
	}
	void handler_read();
	void handler_shutdown();
	void handler_error();
	void handler_write();
	void buffer_no_empty();

	Reactor *_reactor;
	Socket _socket;
	int _num_id;
	Event_handler _event_handler;
	Internet_addr _localaddr;
	Internet_addr _peeraddr;

	Message_callback _m_callback;
//	Callback _m_callback;
	Error_callback _e_callback;
//	Callback _e_callback;
	Shutdown_callback _s_callback;
//	Callback _s_callback;
	Remove_callback _r_callback;

	Tcp_connection_weak_ptr _this_weak_ptr;

	Buffer _read_buffer;
	Buffer _write_buffer;

};

}

#endif
