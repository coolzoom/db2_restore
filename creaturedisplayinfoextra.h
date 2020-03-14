#pragma once
#include<array>
#include"unknown.h"

namespace wdc3
{

struct creaturedisplayinfoextra:unknown<12>{};

inline auto raceid(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,0,6);
}

inline void raceid(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,0,6,u);
}
inline auto sexid(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,6,2);
}

inline void sexid(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,6,2,u);
}

inline auto classid(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,8,4);
}

inline void classid(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,8,4,u);
}

inline auto skinid(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,12,5);
}

inline void skinid(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,12,5,u);
}

inline auto faceid(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,17,6);
}

inline void faceid(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,17,6,u);
}

inline auto skinface(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,12,11);
}

inline void skinface(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,12,11,u);
}

inline auto skin_to_hair(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,12,16);
}

inline void skin_to_hair(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,12,16,u);
}



inline auto hairstyle(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,23,5);
}
inline void hairstyle(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,23,5,u);
}



inline auto race_to_class(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,0,12);
}

inline void race_to_class(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,0,12,u);
}

inline auto skin_to_facial(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,12,39);
}

inline void skin_to_facial(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,12,39,u);
}

inline auto race_sex(creaturedisplayinfoextra const& e)
{
	return get<std::uint8_t>(e,0,8);
}

inline void race_sex(creaturedisplayinfoextra& e,std::uint8_t u)
{
	set(e,0,8,u);
}

inline auto race_to_facial_skip_class(creaturedisplayinfoextra const& e)
{
	return (static_cast<std::uint64_t>(skin_to_facial(e))<<8)+race_sex(e);
}

inline void race_to_facial_skip_class(creaturedisplayinfoextra& e,std::uint64_t u)
{
	race_sex(e,static_cast<std::uint8_t>(u&((1<<8)-1)));
	skin_to_facial(e,static_cast<std::uint32_t>(u>>=8));
}

inline auto race_to_face_skip_class(creaturedisplayinfoextra const& e)
{
	return (static_cast<std::uint64_t>(skinface(e))<<8)+race_sex(e);
}

inline void race_to_face_skip_class(creaturedisplayinfoextra& e,std::uint64_t u)
{
	race_sex(e,static_cast<std::uint8_t>(u&((1<<8)-1)));
	skinface(e,static_cast<std::uint32_t>(u>>=8));
}

inline auto race_to_skin_skip_class(creaturedisplayinfoextra const& e)
{
	return (static_cast<std::uint64_t>(skinid(e))<<8)+race_sex(e);
}

inline void race_to_skin_skip_class(creaturedisplayinfoextra& e,std::uint64_t u)
{
	race_sex(e,static_cast<std::uint8_t>(u&((1<<8)-1)));
	skinid(e,static_cast<std::uint32_t>(u>>=8));
}

inline auto race_to_facial(creaturedisplayinfoextra const& e)
{
	return get<std::uint64_t>(e,0,39);
}

inline void race_to_facial(creaturedisplayinfoextra& e,std::uint64_t u)
{
	set(e,0,39,u);
}
inline auto race_to_hair_skip_class(creaturedisplayinfoextra const& e)
{
	return (static_cast<std::uint64_t>(skin_to_hair(e))<<8)+race_sex(e);
}

inline void race_to_hair_skip_class(creaturedisplayinfoextra& e,std::uint64_t u)
{
	race_sex(e,static_cast<std::uint8_t>(u&((1<<8)-1)));
	skin_to_hair(e,static_cast<std::uint32_t>(u>>=8));
}


inline auto race_to_skin(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,0,17);
}

inline void race_to_skin(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,0,17,u);
}

inline auto sd(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,42,21);
}

inline void sd(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,42,21,u);
}


inline auto hd(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,63,21);
}

inline void hd(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,63,21,u);
}

template<fast_io::buffer_output_stream ostrm>
inline constexpr void print_define(ostrm& out,creaturedisplayinfoextra const& e)
{
	print(raceid(e),u8"\t",sexid(e),u8"\trf:",race_to_facial(e),u8"\tsd:",sd(e),u8"\thd:",hd(e));
}

}