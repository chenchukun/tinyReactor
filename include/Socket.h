#ifndef SOCKET_H
#define SOCKET_H
#include "Internet_addr.h"
#include <stdint.h>

namespace tiny
{

class Socket
{
public:
	Socket(int fd):_fd(fd){}
	Socket();

	int send(const void*, uint32_t) const;
	int recv(void*, uint32_t) const;

	void set_nonblock(bool block) const;
	void set_reuseaddr(bool reuse) const;

	int get_fd() const { return _fd; }
	void shutdown_write() const
	{
		::shutdown(_fd, SHUT_WR);
	}

	void shutdown_read() const
	{
		::shutdown(_fd, SHUT_RD);
	}

	void shutdown() const
	{
		::shutdown(_fd, SHUT_RDWR);
	}

	void bind(const Internet_addr &addr) const;
	void listen(uint32_t backlog) const ;
	int accept(Internet_addr*) const;

	int get_addr(Internet_addr &addr) const;

private:
	int _fd;
};

}
#endif
