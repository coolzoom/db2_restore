#pragma once
#include<cstddef>
#include<vector>
#include<stdexcept>

namespace wdc3
{
template<typename T>
inline decltype(auto) cvs(const std::string &str,const char * &cstr)
{
	auto r(reinterpret_cast<const T*>(cstr));
	if(str.data()+str.size()<(cstr+sizeof(T)))
		throw std::out_of_range("out of range");
	cstr+=sizeof(T);
	return *r;
}

template<typename T>
inline auto cvs(const std::string &str,const char * &cstr,std::size_t count)
{
	auto r(reinterpret_cast<const T*>(cstr));
	if(str.data()+str.size()<cstr+sizeof(T)*count)
		throw std::out_of_range(fast_io::concat<>("out of range: (size: ",str.size(),") (oor: ",cstr+sizeof(T)*count-str.data(),") count:(",count,")"));
	std::vector<T> vec(r,r+count);
	cstr+=sizeof(T)*count;
	return vec;
}

template<>
inline auto cvs<char>(const std::string &str,const char * &cstr,std::size_t count)
{
	if(str.data()+str.size()<cstr+count)
		throw std::out_of_range(fast_io::concat<>("out of range: (size: ",str.size(),") (oor: ",cstr+count-str.data(),") count:(",count,")"));
	std::string r(cstr,cstr+count);
	cstr+=count;
	return r;
}

template<typename T>
inline decltype(auto) svc(std::string &str,const T& t)
{
	std::size_t size(str.size());
	str.append(reinterpret_cast<const char*>(&t),reinterpret_cast<const char*>(&t+1));
	return *reinterpret_cast<T*>(str.data()+size);
}

template<typename T>
inline decltype(auto) svcs(std::string &str,std::size_t t)
{
	std::size_t size(str.size());
	if(str.capacity()<=str.size()+t*sizeof(T))
		throw std::out_of_range("out_of_range");
	str.append(sizeof(T)*t,0);
	return std::span(reinterpret_cast<T*>(str.data()+size),t);
}

template<typename T>
inline decltype(auto) svc(std::string &str,const std::vector<T>& t)
{
	std::size_t size(str.size());
	if(str.capacity()<=size+t.size()*sizeof(T))
		throw std::out_of_range("out_of_range");
	str.append(reinterpret_cast<const char*>(t.data()),reinterpret_cast<const char*>(t.data()+t.size()));
	return std::span(reinterpret_cast<T*>(str.data()+size),t.size());
}

template<typename T>
inline decltype(auto) svc(std::string &str,const std::vector<T>& t,std::uint32_t &s)
{
	std::size_t size(str.size());
	s=t.size();
	if(str.capacity()<=size+t.size()*sizeof(T))
		throw std::out_of_range("out_of_range");
	str.append(reinterpret_cast<const char*>(t.data()),reinterpret_cast<const char*>(t.data()+t.size()));
	return std::span(reinterpret_cast<T*>(str.data()+size),t.size());
}

inline decltype(auto) svc(std::string &str,const std::string& t,std::uint32_t &s)
{
	std::size_t size(str.size());
	s=t.size();
	if(str.capacity()<=size+t.size())
		throw std::out_of_range("out_of_range");
	str.append(t.data(),t.data()+t.size());
	return std::span(reinterpret_cast<char*>(str.data()+size),t.size());
}

}