#pragma once
#include"db2.h"
#include<fstream>
#include<string>
#include<string_view>

namespace wdc3
{
template<typename T>
inline db2<T> read_db2(std::string_view str)
{
	std::ifstream fin(str.data(),std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	return std::string(std::istreambuf_iterator<char>(fin),std::istreambuf_iterator<char>());
}

template<typename T>
inline void write_db2(std::string_view str,db2<T> const &w)
{
	auto sr(serialize(w));
	std::ofstream fout(str.data(),std::ofstream::binary);
	fout.rdbuf()->sputn(sr.data(),sr.size());
}

}