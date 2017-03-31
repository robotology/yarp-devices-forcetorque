#include <stddef.h>
#include <string.h>

#include "baseTypeEncoding.h"

namespace
{

  int32_t encode8BitValue(const uint8_t* value, uint8_t* binary);
  int32_t encode16BitValue(const uint8_t* value, uint8_t* binary);
  int32_t encode32BitValue(const uint8_t* value, uint8_t* binary);
  int32_t encode64BitValue(const uint8_t* value, uint8_t* binary);

  int32_t decode8BitValue(uint8_t* value, const uint8_t* binary);
  int32_t decode16BitValue(uint8_t* value, const uint8_t* binary);
  int32_t decode32BitValue(uint8_t* value,const  uint8_t* binary);
  int32_t decode64BitValue(uint8_t* value, const uint8_t* binary);

}

namespace LIB
{

// Encoding
int32_t encode(const bool value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode8BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const char value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode8BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const int8_t value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode8BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const int16_t value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode16BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const int32_t value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode32BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const int64_t value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 8)
  return encode64BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const uint8_t value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode8BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const uint16_t value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode16BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const uint32_t value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode32BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const uint64_t value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 8)
  return encode64BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const float value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return encode32BitValue((uint8_t*)(&value), binary);
}

int32_t encode(const double value, uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 8)
  return encode64BitValue((uint8_t*)(&value), binary);
}

// Decoding
int32_t decode(bool& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode8BitValue((uint8_t*)(&value), binary);
}

int32_t decode(char& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode8BitValue((uint8_t*)(&value), binary);
}

int32_t decode(int8_t& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode8BitValue((uint8_t*)(&value), binary);
}

int32_t decode(int16_t& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode16BitValue((uint8_t*)(&value), binary);
}

int32_t decode(int32_t& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode32BitValue((uint8_t*)(&value), binary);
}

int32_t decode(int64_t& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 8)
  return decode64BitValue((uint8_t*)(&value), binary);
}

int32_t decode(uint8_t& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode8BitValue((uint8_t*)(&value), binary);
}

int32_t decode(uint16_t& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode16BitValue((uint8_t*)(&value), binary);
}

int32_t decode(uint32_t& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode32BitValue((uint8_t*)(&value), binary);
}

int32_t decode(uint64_t& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 8)
  return decode64BitValue((uint8_t*)(&value), binary);
}

int32_t decode(float& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4)
  return decode32BitValue((uint8_t*)(&value), binary);
}

int32_t decode(double& value, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 8)
  return decode64BitValue((uint8_t*)(&value), binary);
}

} // namespace LIB

// Private functions
namespace
{
  int32_t encode8BitValue(const uint8_t* value, uint8_t* binary)
  {
    binary[0] = *value;
    return 4;
  }

  int32_t encode16BitValue(const uint8_t* value, uint8_t* binary)
  {
    _16BIT_TO_LITTLE_ENDIAN(value);
    memcpy(binary, value, 2);
    return 4;
  }

  int32_t encode32BitValue(const uint8_t* value, uint8_t* binary)
  {
    _32BIT_TO_LITTLE_ENDIAN(value);
    memcpy(binary, value, 4);
    return 4;
  }

  int32_t encode64BitValue(const uint8_t* value, uint8_t* binary)
  {
    _64BIT_TO_LITTLE_ENDIAN(value);
    memcpy(binary, value, 8);
    return 8;
  }

  int32_t decode8BitValue(uint8_t* value, const uint8_t* binary)
  {
    *value = binary[0];
    return 4;
  }

  int32_t decode16BitValue(uint8_t* value, const uint8_t* binary)
  {
    memcpy(value, binary, 2);
    _16BIT_FROM_LITTLE_ENDIAN(value);
    return 4;
  }

  int32_t decode32BitValue(uint8_t* value, const uint8_t* binary)
  {
    memcpy(value, binary, 4);
    _32BIT_FROM_LITTLE_ENDIAN(value);
    return 4;
  }

  int32_t decode64BitValue(uint8_t* value, const uint8_t* binary)
  {
    memcpy(value, binary, 8);
    _64BIT_FROM_LITTLE_ENDIAN(value);
    return 8;
  }
}
