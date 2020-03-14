#pragma once
#include"db2.h"
#include<string>
#include<string_view>

namespace wdc3
{
template<typename T>
inline db2<T> read_db2(std::string_view path)
{
	fast_io::ibuf_file ibf(path);
	std::string str;
	scan(ibf,fast_io::whole(str));
	return str;
}

template<typename T>
inline void write_db2(std::string_view path,db2<T> const &w)
{
	auto sr(serialize(w));
	fast_io::onative_file obf(path);
	print(obf,sr);
}

}