 #pragma once
#include "ThirdPartyHeadersBegin.h"
#include <exception>
#include <string>
#include <boost/unordered_set.hpp>
#include "ThirdPartyHeadersEnd.h"
#include "ThirdPartyHeadersBegin.h"
#include <boost/array.hpp>
#include "ThirdPartyHeadersEnd.h"
#include "SzlFileLoader.h"
#include "NodeMap.h"
#include "FEZoneInfo.h"
#include "ClassMacros.h"
namespace tecplot { namespace ___3933 { static uint16_t const MAX_ENTRIES_FOR_2_BIT_COMPRESSION = 4; static uint16_t const MAX_ENTRIES_FOR_4_BIT_COMPRESSION = 16; static uint16_t const MAX_ENTRIES_FOR_8_BIT_COMPRESSION = 256; template <typename T> inline T const numBytesFor2BitCompression(T ___2795) { return (___2795+3)>>2; } template <typename T> inline T const numBytesFor4BitCompression(T ___2795) { return (___2795+1)>>1; } inline uint8_t const getValueFrom2BitCompressedArray( uint16_t       indx, uint8_t const* uInt8Array) { uint16_t const byte = indx>>2; uint8_t const bits = uInt8Array[byte]; if ( indx & 0x02 ) { if ( indx & 0x01 ) return (bits&0x03); else return (bits>>2)&0x03; } else { if ( indx & 0x01 ) return (bits>>4)&0x03; else return (bits>>6); } } inline uint8_t const getValueFrom4BitCompressedArray( uint16_t       indx, uint8_t const* uInt8Array) { uint16_t const byte = indx>>1; uint8_t const bits = uInt8Array[byte]; if ( indx & 0x01 ) return (bits&0x0F); else return (bits>>4); } class ___1339 { UNCOPYABLE_CLASS(___1339); public: static uint16_t const MAX_CORNERS_PER_CELL = 8; static uint16_t const MAX_NUM_ENTRIES_IN_SUBZONE = (___2090::MAX_ITEM_OFFSET+1)*MAX_CORNERS_PER_CELL; static uint16_t const MAX_REFERENCED_NODE_SUBZONES = MAX_NUM_ENTRIES_IN_SUBZONE; class Error : public std::exception { public: explicit Error(std::string const& what) : m_what(what) {} ~Error() throw() {}; char const* what() const throw() { return m_what.c_str(); } private: std::string m_what; }; ___1339(); ~___1339(); void ___536( ___2729 ___2723, ___1350 const& ___1349, ___2090::SubzoneOffset_t ___469); ___2090::ItemOffset_t ___2782() const; uint16_t numRefNszs() const; uint16_t numRefPartitions() const; PartitionSubzone const* refPtnNszs() const; uint8_t const* refPtn2BitOffsets() const; uint8_t const* refPtn4BitOffsets() const; uint8_t const* refPtn8BitOffsets() const; uint16_t const* refPtn16BitOffsets() const; uint32_t const* ___3319() const; uint8_t const* refNsz2BitOffsets() const; uint8_t const* ___3321() const; uint8_t const* ___3322() const; uint16_t const* ___3320() const; uint8_t const* ___2764() const; private: ___2090::ItemOffset_t m_numCellsInSubzone; boost::array<uint32_t, MAX_NUM_ENTRIES_IN_SUBZONE> m_refPtnArray; boost::array<uint8_t, (MAX_NUM_ENTRIES_IN_SUBZONE+1)/4> m_refPtn2BitOffsets; boost::array<uint8_t, (MAX_NUM_ENTRIES_IN_SUBZONE+1)/2> m_refPtn4BitOffsets; boost::array<uint8_t,  MAX_NUM_ENTRIES_IN_SUBZONE>      m_refPtn8BitOffsets; boost::array<uint16_t, MAX_NUM_ENTRIES_IN_SUBZONE>      m_refPtn16BitOffsets; uint16_t m_numRefNszs; boost::array<___2090,MAX_NUM_ENTRIES_IN_SUBZONE>                    m_szCoordinateArray; boost::array<___2090::SubzoneOffset_t,MAX_REFERENCED_NODE_SUBZONES> m_refNszArray; boost::array<uint8_t,(MAX_NUM_ENTRIES_IN_SUBZONE+1)/4>  m_refNsz2BitOffsets; boost::array<uint8_t,(MAX_NUM_ENTRIES_IN_SUBZONE+1)/2>  m_refNsz4BitOffsets; boost::array<uint8_t,MAX_NUM_ENTRIES_IN_SUBZONE>        m_refNsz8BitOffsets; boost::array<uint16_t,MAX_NUM_ENTRIES_IN_SUBZONE>       m_refNsz16BitOffsets; boost::array<uint8_t,MAX_NUM_ENTRIES_IN_SUBZONE>        m_nszOffsets; uint16_t m_numRefPartitions; boost::array<PartitionSubzone, MAX_NUM_ENTRIES_IN_SUBZONE> m_ptnNszReferences; PartitionSubzoneSet m_refPtnNszSet; boost::array<PartitionSubzone, MAX_NUM_ENTRIES_IN_SUBZONE> m_refPtnNszArray; }; }}
