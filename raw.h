#pragma once
#include<array>
#include<exception>
#include<string>
#include"cvs.h"

namespace wdc3
{
struct header
{
	std::uint32_t record_count;           // this is for all sections combined now
	std::uint32_t field_count;
	std::uint32_t record_size;
	std::uint32_t string_table_size;      // this is for all sections combined now
	std::uint32_t table_hash;             // hash of the table name
	std::uint32_t layout_hash;            // this is a hash field that changes only when the structure of the data changes
	std::uint32_t min_id;
	std::uint32_t max_id;
	std::uint32_t locale;                 // as seen in TextWowEnum
	std::uint16_t flags;                  // possible values are listed in Known Flag Meanings
	std::uint16_t id_index;               // this is the index of the field containing ID values; this is ignored if flags & 0x04 != 0
	std::uint32_t total_field_count;      // from WDC1 onwards, this value seems to always be the same as the 'field_count' value
	std::uint32_t bitpacked_data_offset;  // relative position in record where bitpacked data begins; not important for parsing the file
	std::uint32_t lookup_column_count;
	std::uint32_t field_storage_info_size;
	std::uint32_t common_data_size;
	std::uint32_t pallet_data_size;
	std::uint32_t section_count;          // new to WDC2, this is number of sections of data (records + copy table + id list + relationship map = a section)
};

struct section_header
{
	std::uint64_t tact_key_hash;       // TactKeyLookup hash
	std::uint32_t file_offset;            // absolute position to the beginning of the section
	std::uint32_t record_count;           // 'record_count' for the section
	std::uint32_t string_table_size;      // 'string_table_size' for the section
	std::uint32_t offset_records_end;	// Offset to the spot where the records end in a file with an offset map structure;
	std::uint32_t id_list_size;           // List of ids present in the DB file
	std::uint32_t relationship_data_size;// Size of the relationship data in the section
	std::uint32_t offset_map_id_count;// Count of ids present in the offset map in the section
	std::uint32_t copy_table_count;// Count of the number of deduplication entries (you can multiply by 8 to mimic the old 'copy_table_size' field)
};

template<fast_io::buffer_output_stream ostrm>
inline constexpr void print_define(ostrm& o,const section_header& fs)
{
	print(o,u8"tact_key_hash:",fs.tact_key_hash,
			u8"\nfile_offset:",fs.file_offset,
			u8"\nrecord_count:",fs.record_count,
			u8"\nstring_table_size:",fs.string_table_size,
			u8"\noffset_records_end:",fs.offset_records_end,
			u8"\nid_list_size:",fs.id_list_size,
			u8"\nrelationship_data_size:",fs.relationship_data_size,
			u8"\noffset_map_id_count:",fs.offset_map_id_count,
			u8"\ncopy_table_count:",fs.copy_table_count);
}

template<typename T>
inline decltype(auto) check_section_validity(const std::string& str,const char * &p,const section_header& s)
{
	if(str.data()+s.file_offset!=p)
		throw std::runtime_error(fast_io::concat<>("offset error size: ",str.size()," pos: ",p-str.data(),
						" should be: ",s.file_offset));
	return cvs<T>(str,p,s.record_count);
}

struct field_structure
{
	std::uint16_t size;
	std::uint16_t offset;
};

template<fast_io::buffer_output_stream ostrm>
inline constexpr void print_define(ostrm& o,const field_structure& fs)
{
	print(o,fs.offset,u8"\tsize:",(32-fs.size)>>3);
}

enum class field_compression
{
	// None -- the field is a 8-, 16-, 32-, or 64-bit integer in the record data
	none,
	// Bitpacked -- the field is a bitpacked integer in the record data.  It
	// is field_size_bits long and starts at field_offset_bits.
	// A bitpacked value occupies
	//   (field_size_bits + (field_offset_bits & 7) + 7) / 8
	// bytes starting at byte
	//   field_offset_bits / 8
	// in the record data.  These bytes should be read as a little-endian value,
	// then the value is shifted to the right by (field_offset_bits & 7) and
	// masked with ((1ull << field_size_bits) - 1).
	bitpacked,
	// Common data -- the field is assumed to be a default value, and exceptions
	// from that default value are stored in the corresponding section in
	// common_data as pairs of { std::uint32_t record_id; std::uint32_t value; }.
	common_data,
	// Bitpacked indexed -- the field has a bitpacked index in the record data.
	// This index is used as an index into the corresponding section in
	// pallet_data.  The pallet_data section is an array of std::uint32_t, so the index
	// should be multiplied by 4 to obtain a byte offset.
	bitpacked_indexed,
	// Bitpacked indexed array -- the field has a bitpacked index in the record
	// data.  This index is used as an index into the corresponding section in
	// pallet_data.  The pallet_data section is an array of std::uint32_t[array_count],
	//
	bitpacked_indexed_array,
	// Same as field_compression_bitpacked
	bitpacked_signed
};

template<fast_io::buffer_output_stream ostrm>
inline constexpr void print_define(ostrm& o,const field_compression& fs)
{
	switch(fs)
	{
		case field_compression::none: print(o,u8"none");
		case field_compression::bitpacked: print(o,u8"bitpacked");
		case field_compression::common_data: print(o,u8"common data");
		case field_compression::bitpacked_indexed: print(o,u8"bitpacked_indexed");
		case field_compression::bitpacked_indexed_array: print(o,u8"bitpacked_indexed_array");
		case field_compression::bitpacked_signed: print(o,u8"bitpacked_signed");
		default: print(o,u8"unknown(",static_cast<unsigned>(fs),u8")");
	}
}

struct field_storage_info
{
	std::uint16_t offset_bits;
	std::uint16_t field_size;
	std::uint32_t additional_data_size;
	field_compression type;
	std::array<std::uint32_t,3> values;
};

template<fast_io::buffer_output_stream ostrm>
inline constexpr void print_define(ostrm& o,const field_storage_info& fs)
{
	print(o,u8"offset_bits\t",fs.offset_bits,
		u8"\nfield_size\t",fs.field_size,
		u8"\nadditional_data_size\t",fs.additional_data_size,
		u8"\ntype\t",fs.type);
	switch(fs.type)
	{
	case field_compression::none:break;
	case field_compression::bitpacked:
		print(o,u8"\n\nbitpacking_offset_bits\t",fs.values.front(),
		u8"\nbitpacking_size_bits\t",fs.values[1],
		u8"\nflags\t",fs.values[2]);
	break;
	case field_compression::common_data:
		print(o,u8"\n\ndefault_value\t",fs.values.front());
	break;
	case field_compression::bitpacked_indexed:
		print(o,u8"\n\nbitpacking_offset_bits\t",fs.values.front(),
		o,u8"\nbitpacking_size_bits\t",fs.values[1]);
	break;
	case field_compression::bitpacked_indexed_array:
		print(o,u8"\n\nbitpacking_offset_bits\t",fs.values.front(),
		u8"\nbitpacking_size_bits\t",fs.values[1],
		u8"\narray_count\t",fs.values[2]);
	break;
	default:
		print(o,u8"\n\n");
		for(std::size_t i(0);i!=fs.values.size();++i)
			print(o,fs.values[i],u8"\t");
	}
}

struct copy_table_entry
{
	std::uint32_t id_of_new_row;
	std::uint32_t id_of_copied_row;
};

struct offset_map_entry
{
	std::uint32_t offset;                                          // this offset is absolute, not relative to another structure; this can (and often will) be zero, in which case you should ignore that entry and move on
	std::uint16_t length;                                          // this is the length of the record located at the specified offset
};

struct relationship_entry
{
	// This is the id of the foreign key for the record, e.g. SpellID in
	// SpellX* tables.
	std::uint32_t foreign_id;
	// This is the index of the record in record_data.  Note that this is
	// *not* the record's own ID.
	std::uint32_t record_index;
};

template<fast_io::buffer_output_stream ostrm>
inline constexpr void print_define(ostrm& o,relationship_entry const& fs)
{
	print(o,fs.foreign_id,u8" ",fs.record_index);
}

struct relationship_mapping_header
{
	std::uint32_t num_entries;
	std::uint32_t min_id;
	std::uint32_t max_id;
};

}