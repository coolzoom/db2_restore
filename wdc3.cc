#include<fast_io.h>
#include<fast_io_device.h>
#include"db2_io.h"
#include"creaturedisplayinfoextra.h"
#include"creaturedisplayinfo.h"
#include"chrraces.h"
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<random>

std::unordered_set<std::uint32_t> both;

inline auto stage1()
{
	wdc3::db2<wdc3::creaturedisplayinfoextra> creaturedisplayinfoextra(wdc3::read_db2<wdc3::creaturedisplayinfoextra>("creaturedisplayinfoextra.db2"));
	std::unordered_map<std::uint8_t,std::vector<std::size_t>> racesex_lookup;
	auto &records(creaturedisplayinfoextra.sections.front().records);
	for(std::size_t i(0);i!=records.size();++i)
	{
		auto const &e (records[i]);
		if(sd(e)&&hd(e))
		{
			racesex_lookup[race_sex(e)].emplace_back(i);
			both.emplace(creaturedisplayinfoextra.sections.front().ids.at(i));
		}
	}
	std::random_device device;
	std::size_t count(0);
	for(auto & e : records)
	{
		if(!sd(e)&&hd(e))
		{
			auto lookup_vector(racesex_lookup.at(race_sex(e)));
			std::uniform_int_distribution<std::size_t> distribution(0,lookup_vector.size()-1);
			sd(e,sd(records.at(lookup_vector[distribution(device)])));
			++count;
		}
	}
	println("Stage 1 Done: SD Textures which does not exist in creaturedisplayinfoextra: ",count);
	return creaturedisplayinfoextra;
}

inline auto convert(std::vector<wdc3::creaturedisplayinfo> &records)
{
	std::unordered_map<std::uint32_t,std::size_t> um;
	for(std::size_t i(0);i!=records.size();++i)
	{
		auto &ele(records[i]);
		um[id(ele)]=i;
	}
	return um;
}

inline auto stage2()
{
	auto chrraces(wdc3::read_db2<wdc3::chrraces>("chrraces.db2"));
	auto &records(chrraces.sections.at(0).records);
	std::unordered_map<std::uint32_t,std::uint32_t> new_to_old_map;
	std::unordered_map<std::uint32_t,std::uint32_t> old_to_new_map;
	auto creaturedisplayinfo(wdc3::read_db2<wdc3::creaturedisplayinfo>("creaturedisplayinfo.db2"));

	auto creature_id_to_index(convert(creaturedisplayinfo.sections.at(0).records));
	auto &cr(creaturedisplayinfo.sections.at(0).records);

	for(auto const & e : records)
	{
		auto hdm(hd_male(e)),hdfm(hd_female(e));
		if(hdm)
		{
			auto sd(model(cr.at(creature_id_to_index.at(sd_male(e)))));
			auto hd(model(cr.at(creature_id_to_index.at(hdm))));
			old_to_new_map[sd]=hd;
			new_to_old_map[hd]=sd;
		}
		if(hdfm)
		{
			auto sd(model(cr.at(creature_id_to_index.at(sd_female(e)))));
			auto hd(model(cr.at(creature_id_to_index.at(hdfm))));
			old_to_new_map[sd]=hd;
			new_to_old_map[hd]=sd;
		}
	}
	std::size_t incorrect_hd_sd_textures(0);
	for(auto & e : creaturedisplayinfo.sections.at(0).records)
	{
		auto ex(extra(e));
		if(ex&&both.count(ex))
		{
			auto it(new_to_old_map.find(model(e)));
			if(it!=new_to_old_map.cend())
			{
				model(e,it->second);
				++incorrect_hd_sd_textures;
			}
		}
	}
	print("Stage 2 : fixed ",incorrect_hd_sd_textures," incorrect hd sd textures \n");
	return creaturedisplayinfo;
}

inline auto stage3(int argc,char **argv)
{
	wdc3::db2<wdc3::chrraces> chrraces(wdc3::read_db2<wdc3::chrraces>("chrraces.db2"));
	auto &records(chrraces.sections.front().records);
	for(std::size_t i(1),argc_s(argc);i<argc_s;++i)
	{
		std::string s(argv[i]);
		for(auto &ele : s)
			ele=std::tolower(ele);
		if(s=="hum"||s=="human"||s=="worgen")
		{
			enable_old_model(records.at(0));	//enable human
			enable_old_model(records.at(22));	//enable worgen since they share same files		
		}
//		else if(s=="orc")
//		{
//			enable_old_model(records.at(1));	//enable orc not usable
//			upright_orc = true;				//disable upright orc
//		}
		else if(s=="dwarf")
			enable_old_model(records.at(2));	//enable dwarf
		else if(s=="ne"||s=="nightelf")
			enable_old_model(records.at(3));	//enable ne
//		else if(s=="tauren")
//			enable_old_model(records.at(5));	//enable tauren NOT USABLE
		else if(s=="gnome")
			enable_old_model(records.at(6));	//enable gnome
		else if(s=="troll")
			enable_old_model(records.at(7));	//enable troll
		else if(s=="bloodelf"||s=="be")
			enable_old_model(records.at(9));	//enable be
		else if(s=="draenei")
			enable_old_model(records.at(10));	//enable draenei
		else if(s=="undead")
			enable_old_model(records.at(4));	//enable undead
		else
			throw std::runtime_error(fast_io::concat<>("race: ",s," does not exist"));
	}
	print("Stage 3 Done: Deflagging SD model disable toggle inside chrraces.db2\n");
	return chrraces;
}

int main(int argc,char** argv)
try
{
	if(argc==1)
	{
		print(fast_io::err,"Usage: ",*argv," <race name1> <race name2>...\n");
		return 1;
	}
	print("Notice that the arctium wow launcher does neither support adding custom filedataids nor modifying creaturedisplayinfo. It would be a huge problem here.\n");
	write_db2("creaturedisplayinfoextra.db2.fixed",stage1());
	write_db2("chrraces.db2.fixed",stage3(argc,argv));
	write_db2("creaturedisplayinfo.db2.fixed",stage2());
}
catch(std::exception const&ex)
{
	println(fast_io::err,ex);
	return 1;
}