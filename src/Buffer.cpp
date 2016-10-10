#include "Buffer.h"
#include <cstring>
#include <iostream>

namespace tiny
{

size_t Buffer::size()
{
	return _end_index - _begin_index;
}

char* Buffer::head()
{
	return &_buf[_begin_index];
}

void Buffer::discard_nbyte(size_t n)
{
	_begin_index += n;
	if(_begin_index >= _end_index)
	{
		_begin_index = _end_index = 0;
	}
}

void Buffer::read(void *buf, size_t n)
{
	if(n > size())
	{
		n = size();
	}
	memcpy(buf, static_cast<void*>(&_buf[_begin_index]), n);
	_begin_index += n;
	if(_begin_index == _end_index)
	{
		_begin_index = _end_index = 0;
	}
}

void Buffer::write(const void *buf, size_t n)
{
//	std::cout << "Buffer debug: " << _buf.size() << std::endl;
	const char *p = static_cast<const char*>(buf);
	size_t i = 0;
	while(_end_index<_buf.size() && i<n)
	{
		_buf[_end_index++] = p[i++];
	}

	while(i < n)
	{
		_buf.push_back(p[i++]);
		++_end_index;
	}

	if(_write_callback != NULL)
	{
		_write_callback();
	}
}

void Buffer::read(std::string &buf)
{
	buf = std::move(std::string(head(), size()));
	_begin_index = _end_index = 0;
}

void Buffer::write(const std::string &buf)
{
	write(&buf[0], buf.size());
}

void Buffer::read(std::string &buf, size_t n)
{
	if(n > size())
	{
		n = size();
	}
	buf = std::move(std::string(head(), n));
	_begin_index += n;
	if(_begin_index == _end_index)
	{
		_begin_index = _end_index = 0;
	}
}

}
