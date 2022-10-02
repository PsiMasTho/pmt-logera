#include "log_data.h"

using namespace std;

bool LogData::const_iterator::operator==(const_iterator const& other) const
{
	if(isEnd())
		return other.isEnd();
  	if(other.isEnd())
		return false;

	return (d_mapItr == other.d_mapItr) && (d_vecIdx && other.d_vecIdx);
}

bool LogData::const_iterator::operator!=(const_iterator const& other) const
{
	return !operator==(other);
}

	// prefix
LogData::const_iterator& LogData::const_iterator::operator++()
{
	if (d_vecIdx < d_mapItr->second.size() - 1)
		++d_vecIdx;
	else
	{
		++d_mapItr; // will automatically reach d_parent->d_dateToLogLineMap.end();
		d_vecIdx = 0;
	}

	if (!isEnd())
	{
		d_value.first = d_mapItr->first;
		d_value.second = d_mapItr->second[d_vecIdx];
	}
	return *this;
}

	// postfix
LogData::const_iterator LogData::const_iterator::operator++(int)
{
	const_iterator tmp(*this);
  	operator++(); // prefix-increment this
  	return tmp;   // return value before increment
}

pair<Date, LogLine> const& LogData::const_iterator::operator*() const
{
	return d_value;
}

LogData::const_iterator::const_iterator(LogData* parent, map<Date, vector<LogLine>>::const_iterator itr)
:
	d_parent{parent},
	d_mapItr{itr},
	d_vecIdx{0},
	d_value{pair{"01Jan0001", LogLine{}}} // placeholder
{
	if (!isEnd())
	{
		d_value.first = d_mapItr->first;
		d_value.second = d_mapItr->second[d_vecIdx];
	}
}

bool LogData::const_iterator::isEnd() const
{
	return d_mapItr == d_parent->d_dateToLogLineMap.end();
}
