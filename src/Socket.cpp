#include "Socket.h"
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <cstring>

namespace tiny
{

Socket::Socket()
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(-1 != _fd);
}

int Socket::send(const void *buf, uint32_t size) const
{
	int n = ::write(_fd, buf, size);
	return n;
}
int Socket::recv(void *buf, uint32_t size) const
{
	int n = ::read(_fd, buf, size);
	return n;
}
void Socket::set_nonblock(bool block) const
{
	int flag = fcntl(_fd, F_GETFL);
	if(block)
	{
		flag |= O_NONBLOCK;
	}
	else
	{
		flag &= ~O_NONBLOCK;
	}
	int ret = fcntl(_fd, F_SETFL, flag);
	assert(-1 != ret);
}
void Socket::set_reuseaddr(bool reuse) const
{
	int r = reuse==true?1:0;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &r, sizeof(int));
}

void Socket::bind(const Internet_addr &addr) const
{
	int ret = ::bind(_fd, addr.get_sockaddr(), sizeof(*(addr.get_sockaddr())));
	assert(-1 != ret);
}
void Socket::listen(uint32_t backlog) const
{
	int ret = ::listen(_fd, backlog);
	assert(-1 != ret);
}

int Socket::accept(Internet_addr *addr) const
{
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	// 开头的::表示应用全局的函数
	int fd = ::accept(_fd, (struct sockaddr*)&cliaddr, &len);
	Internet_addr tmp_addr = Internet_addr(&cliaddr);
	*addr = tmp_addr;
	return fd;
}

int Socket::get_addr(Internet_addr &addr) const
{
	struct sockaddr_in localaddr;
	bzero(&localaddr, sizeof(localaddr));

	socklen_t addrlen = sizeof(localaddr);

	int ret = ::getsockname(_fd, (struct sockaddr*)(&localaddr), &addrlen);
	if(ret != -1)
	{
		addr = Internet_addr(&localaddr);
	}
	return ret;
}

}
