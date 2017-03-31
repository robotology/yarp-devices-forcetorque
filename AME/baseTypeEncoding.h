#ifndef LIBCPPBASETYPEENCODER_32BITALIGNED_H_INCLUDED
#define LIBCPPBASETYPEENCODER_32BITALIGNED_H_INCLUDED

#include <stdint.h>

#define CHECK_BUFFER_SIZE(maxSize, requestedSize) \
  do { \
    if( (maxSize) < (requestedSize) ) \
      return -1; \
  } while(0);

#ifdef BIG_ENDIAN
  #include <byteswap.h>

  #define _16BIT_TO_LITTLE_ENDIAN(x)  *x = bswap_16(*x);
  #define _32BIT_TO_LITTLE_ENDIAN(x)  *x = bswap_32(*x);
  #define _64BIT_TO_LITTLE_ENDIAN(x)  *x = bswap_64(*x);

  #define _16BIT_FROM_LITTLE_ENDIAN(x)  *x = bswap_16(*x);
  #define _32BIT_FROM_LITTLE_ENDIAN(x)  *x = bswap_32(*x);
  #define _64BIT_FROM_LITTLE_ENDIAN(x)  *x = bswap_64(*x);
#else
  #define _16BIT_TO_LITTLE_ENDIAN(x)
  #define _32BIT_TO_LITTLE_ENDIAN(x)
  #define _64BIT_TO_LITTLE_ENDIAN(x)

  #define _16BIT_FROM_LITTLE_ENDIAN(x)
  #define _32BIT_FROM_LITTLE_ENDIAN(x)
  #define _64BIT_FROM_LITTLE_ENDIAN(x)
#endif // BIG_ENDIAN

#define ENCODING_TYPE 0x1

namespace LIB 
{

int32_t encode(const bool value, uint8_t* binary, const size_t maxSize);
int32_t encode(const char value, uint8_t* binary, const size_t maxSize);
int32_t encode(const int8_t value, uint8_t* binary, const size_t maxSize);
int32_t encode(const int16_t value, uint8_t* binary, const size_t maxSize);
int32_t encode(const int32_t value, uint8_t* binary, const size_t maxSize);
int32_t encode(const int64_t value, uint8_t* binary, const size_t maxSize);
int32_t encode(const uint8_t value, uint8_t* binary, const size_t maxSize);
int32_t encode(const uint16_t value, uint8_t* binary, const size_t maxSize);
int32_t encode(const uint32_t value, uint8_t* binary, const size_t maxSize);
int32_t encode(const uint64_t value, uint8_t* binary, const size_t maxSize);
int32_t encode(const float value, uint8_t* binary, const size_t maxSize);
int32_t encode(const double value, uint8_t* binary, const size_t maxSize);

int32_t decode(bool& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(char& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(int8_t& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(int16_t& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(int32_t& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(int64_t& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(uint8_t& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(uint16_t& value,const  uint8_t* binary, const size_t maxSize);
int32_t decode(uint32_t& value,const  uint8_t* binary, const size_t maxSize);
int32_t decode(uint64_t& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(float& value, const uint8_t* binary, const size_t maxSize);
int32_t decode(double& value, const uint8_t* binary, const size_t maxSize);

} // namespace LIB

#endif // LIBCPPBASETYPEENCODER_32BITALIGNED_H_INCLUDED
