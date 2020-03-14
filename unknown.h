#pragma once
#include<array>
#include<cstdint>
#include<bitset>

namespace wdc3
{
template<std::size_t size>
struct unknown
{
	std::array<std::uint8_t,size> a;
};

template<fast_io::buffer_output_stream ostrm,std::size_t T>
inline constexpr void print(ostrm& out,unknown<T> const& c)
{
	for(std::size_t i(0);i!=c.a.size();++i)
	{
		if(i)
			put(out,u8' ');
		print(out,static_cast<std::uint16_t>(c.a[i]));
	}
}

template<typename T,std::size_t N>
inline auto get(unknown<N> const& unk,std::size_t B,std::size_t size)
{
	std::bitset<8*N> b;
	memcpy(std::addressof(b),std::addressof(unk),N);
	T v(0);
	for(std::size_t i(0);i!=size;++i)
		v|=(b.test(i+B)<<i);
	return v;
}

template<std::size_t N,typename T>
inline void set(unknown<N>& unk,std::size_t B,std::size_t size,T t)
{
	std::bitset<8*N> b;
	memcpy(std::addressof(b),std::addressof(unk),N);
	for(std::size_t i(B),e(B+size);i!=e;++i,(t>>=1))
		b.set(i,t&1);
	memcpy(std::addressof(unk),std::addressof(b),N);
}


}