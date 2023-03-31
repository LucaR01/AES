#ifndef AES_CUSTOM_TYPES_HPP
#define AES_CUSTOM_TYPES_HPP

#include <string>
#include <cstdint>
#include <sstream>

namespace aes::ct {
	
//TODO: dal C++20 esiste std::u8string che è std::basic_string<char8_t> e char8_t è un typedef su unsigned char.
	
typedef std::basic_string<uint8_t> ustring;

typedef std::basic_stringstream<uint8_t> u8stringstream;

using ufstream = std::basic_fstream<uint8_t, std::char_traits<uint8_t>>;


/*std::basic_istream& operator>>( unsigned char& value);

std::basic_istream& operator<<( uint8_t& value );*/
	
} // namespace aes::ct

#endif //AES_CUSTOM_TYPES_HPP