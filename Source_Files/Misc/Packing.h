#ifndef _PACKING_
#define _PACKING_
/*
	Created by Loren Petrich, August 28, 2000

	Basic packing and unpacking routines. These are used because the Marathon series has used
	packed big-endian data formats, while internally, data is most efficiently accessed
	if it has native alignments, which often involve compiler-generated padding.
	Furthermore, the data may internally be little-endian instead of big-endian.

	The packed form of the data is a simple stream of bytes, and as the routines run,
	they advance the stream pointer appropriately. So be sure to keep a copy of an object's
	base pointer when using these routines.
	
	The syntax was chosen to make it easy to create an unpacking version of a packing routine,
	by simply changing the routine names.
	
	StreamToValue(uint8* &Stream, T& Number)
		unpacks a stream into a numerical value (T is int16, uint16, int32, uint32)
	
	ValueToStream(uint8* &Stream, T Number)
		packs a numerical value (T is int16, uint16, int32, uint32) into a stream
	
	StreamToList(uint8* &Stream, T* List, int Count)
		unpacks a stream into a list of numerical values (T is int16, uint16, int32, uint32)
	
	ListToStream(uint8* &Stream, const T* List, int Count)
		packs a list of numerical values (T is int16, uint16, int32, uint32) into a stream
	
	StreamToBytes(uint8* &Stream, void* Bytes, int Count)
		unpacks a stream into a block of bytes
	
	BytesToStream(uint8* &Stream, const void* Bytes, int Count)
		packs a block of bytes into a stream
*/

#include <string.h>

#define PACKED_DATA_IS_BIG_ENDIAN

#if !(defined(PACKED_DATA_IS_BIG_ENDIAN)) && !(defined(PACKED_DATA_IS_LITTLE_ENDIAN))
#error "At least one of PACKED_DATA_IS_BIG_ENDIAN and PACKED_DATA_IS_LITTLE_ENDIAN must be defined!"
#elif (defined(PACKED_DATA_IS_BIG_ENDIAN)) && (defined(PACKED_DATA_IS_LITTLE_ENDIAN))
#error "PACKED_DATA_IS_BIG_ENDIAN and PACKED_DATA_IS_LITTLE_ENDIAN cannot both be defined at the same time!"
#endif

inline void StreamToValue(uint8* &Stream, uint16 &Value)
{
	// Must be unsigned, so they will be zero-extended
	uint16 Byte0 = uint16(*(Stream++));
	uint16 Byte1 = uint16(*(Stream++));

#if defined(PACKED_DATA_IS_BIG_ENDIAN)
	Value = (Byte0 << 8) | Byte1;
#elif defined(PACKED_DATA_IS_LITTLE_ENDIAN)
	Value = (Byte1 << 8) | Byte0;
#endif
}

inline void StreamToValue(uint8* &Stream, int16 &Value)
{
	uint16 UValue;
	StreamToValue(Stream,UValue);
	Value = int16(UValue);
}

inline void StreamToValue(uint8* &Stream, uint32 &Value)
{
	// Must be unsigned, so they will be zero-extended
	uint32 Byte0 = uint32(*(Stream++));
	uint32 Byte1 = uint32(*(Stream++));
	uint32 Byte2 = uint32(*(Stream++));
	uint32 Byte3 = uint32(*(Stream++));

#if defined(PACKED_DATA_IS_BIG_ENDIAN)
	Value = (Byte0 << 24) | (Byte1 << 16) | (Byte2 << 8) | Byte3;
#elif defined(PACKED_DATA_IS_LITTLE_ENDIAN)
	Value = (Byte3 << 24) | (Byte2 << 16) | (Byte1 << 8) | Byte0;
#endif
}

inline void StreamToValue(uint8* &Stream, int32 &Value)
{
	uint32 UValue;
	StreamToValue(Stream,UValue);
	Value = int32(UValue);
}

inline void ValueToStream(uint8* &Stream, uint16 Value)
{
#if defined(PACKED_DATA_IS_BIG_ENDIAN)
	*(Stream++) = uint8(Value >> 8);
	*(Stream++) = uint8(Value);
#elif defined(PACKED_DATA_IS_LITTLE_ENDIAN)
	*(Stream++) = uint8(Value);
	*(Stream++) = uint8(Value >> 8);
#endif
}

inline void ValueToStream(uint8* &Stream, int16 Value)
{
	ValueToStream(Stream,uint16(Value));
}

inline void ValueToStream(uint8* &Stream, uint32 Value)
{
#if defined(PACKED_DATA_IS_BIG_ENDIAN)
	*(Stream++) = uint8(Value >> 24);
	*(Stream++) = uint8(Value >> 16);
	*(Stream++) = uint8(Value >> 8);
	*(Stream++) = uint8(Value);
#elif defined(PACKED_DATA_IS_LITTLE_ENDIAN)
	*(Stream++) = uint8(Value);
	*(Stream++) = uint8(Value >> 8);
	*(Stream++) = uint8(Value >> 16);
	*(Stream++) = uint8(Value >> 24);
#endif
}

inline void ValueToStream(uint8* &Stream, int32 Value)
{
	ValueToStream(Stream,uint32(Value));
}

template<class T> void StreamToList(uint8* &Stream, T* List, int Count)
{
	T* ValuePtr = List;
	for (int k=0; k<Count; k++)
		StreamToValue(Stream,*(ValuePtr++));
}


template<class T> void ListToStream(uint8* &Stream, T* List, int Count)
{
	T* ValuePtr = List;
	for (int k=0; k<Count; k++)
		ValueToStream(Stream,*(ValuePtr++));
}

inline void StreamToBytes(uint8* &Stream, void* Bytes, int Count)
{
	memcpy(Bytes,Stream,Count);
	Stream += Count;
}

inline void BytesToStream(uint8* &Stream, const void* Bytes, int Count)
{
	memcpy(Stream,Bytes,Count);
	Stream += Count;
}

#endif
