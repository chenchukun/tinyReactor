#ifndef TIME_STAMP_H
#define TIME_STAMP_H

#include <string>
#include <stdint.h>

namespace tiny
{

class Time_stamp
{
public:
	enum MICRO{MICRO_SECOND=10000000};
	Time_stamp(uint64_t second, uint64_t msecond=0)
		: _microsecond(second*MICRO_SECOND + msecond)
	{
	}
	std::string to_string() const;
	std::string to_formt() const;
	uint64_t get_microsecond() const;

	bool operator==(const Time_stamp &stamp) const;
	bool operator!=(const Time_stamp &stamp) const ;
	bool operator>(const Time_stamp &stamp) const;
	bool operator>=(const Time_stamp &stamp) const;
	bool operator<(const Time_stamp &stamp) const;
	bool operator<=(const Time_stamp &stamp) const;

	Time_stamp operator+(const Time_stamp &t) const
	{
		Time_stamp ret(0, _microsecond+t._microsecond);
		return ret;
	}
	static Time_stamp now();

private:
	uint64_t _microsecond;
};

}

#endif


