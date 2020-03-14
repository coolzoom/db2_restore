#pragma once
#include<array>
#include<bitset>

namespace wdc3
{

struct chrraces
{
	std::array<std::int32_t,33> a;
	std::uint32_t unk1;
	std::array<std::int16_t,4> b;
	std::array<std::int8_t,16> c;
//	std::uint32_t race;
//	std::array<std::uint32_t,14> 
//	std::array<std::uint32_t,40> a;
};


inline constexpr decltype(auto) raceid(chrraces const& c)
{
	return c.a[6];
}

inline constexpr decltype(auto) raceid(chrraces& c)
{
	return c.a[6];
}


inline constexpr decltype(auto) hd_male(chrraces const& c)
{
	return c.a[10];
}

inline constexpr decltype(auto) hd_female(chrraces const& c)
{
	return c.a[11];
}

inline constexpr decltype(auto) sd_male(chrraces const& c)
{
	return c.a[8];
}
inline constexpr decltype(auto) sd_female(chrraces const& c)
{
	return c.a[9];
}

inline constexpr decltype(auto) hd_male(chrraces& c)
{
	return c.a[10];
}
inline constexpr decltype(auto) hd_female(chrraces& c)
{
	return c.a[11];
}

inline constexpr decltype(auto) sd_male(chrraces& c)
{
	return c.a[8];
}
inline constexpr decltype(auto) sd_female(chrraces& c)
{
	return c.a[9];
}


inline constexpr decltype(auto) flags(chrraces& c)
{
	return c.a[7];
}

inline constexpr void enable_old_model(chrraces& c)
{
	flags(c)&=0x80u-1;
}

inline constexpr void disable_old_model(chrraces& c)
{
	flags(c)|=0x80u;
}

inline constexpr decltype(auto) flags(chrraces const& c)
{
	return c.a[7];
}

template<fast_io::buffer_output_stream ostrm>
constexpr void print_define(ostrm& out,chrraces const& e)
{
	print(out,raceid(e),u8" ");
	if(flags(e)&0x8)
		print(out,u8"(Playable)");
	if(flags(e)&0x80)
		print(out,u8"(Old models are disabled)");
	print(out,u8"\tsd male:",sd_male(e),u8," sd female:",sd_female(e),
	u8"\thd male:",hd_male(e),u8"\thd female:",hd_female(e));
	return out;
}
}