#include "MultitorqueEncoder.h"

#include "baseTypeEncoding.h"

namespace Multitorque
{

using LIB::encode;
using LIB::decode;

int32_t packHeaderInfo(uint16_t messageId, uint16_t messageSize, uint8_t messageEncoding, uint8_t* binary)
{
  uint32_t value = 0;

  value |= ((messageId & 0x0fff) << 20);
  value |= messageSize;
  value |= ((messageEncoding & 0x0f) << 16);

  _32BIT_TO_LITTLE_ENDIAN(value);
  memcpy(binary, &value, 4);

  return 4;
}

int32_t unpackHeaderInfo(LIBHeader& header, const uint8_t* binary, const size_t maxSize)
{
  CHECK_BUFFER_SIZE(maxSize, 4);

  uint32_t value = 0;
  uint16_t idValue;
  uint16_t sizeValue;
  uint8_t encodingValue;

  memcpy(&value, binary, 4);
  _32BIT_FROM_LITTLE_ENDIAN(value);

  idValue = ((value & 0xfff00000) >> 20);
  sizeValue = ((value & 0x0000ffff));
  encodingValue = ((value & 0x000f0000) >> 16);

  header.setId(idValue);
  header.setSize(sizeValue);
  header.setEncoding(encodingValue);

  return 4;
}

// Header
int32_t encode(const LIBHeader& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // Skip packed auto-filled informations
  bytes += 4;

  return bytes;
}

int32_t decode(LIBHeader& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // Set the automatic header fields
  retCode = unpackHeaderInfo(message, binary, maxSize);
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  return bytes;
}

// GetBoardInfo
int32_t encode(const GetBoardInfo& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_BOARD_INFO, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetBoardInfo& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyBoardInfo
int32_t encode(const ReplyBoardInfo& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // release
  retCode = encode(message.getRelease(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_BOARD_INFO, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyBoardInfo& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // release
  {
    uint32_t r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRelease(r);
  }

  return bytes;
}

// CalibrateOffsets
int32_t encode(const CalibrateOffsets& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__CALIBRATE_OFFSETS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(CalibrateOffsets& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyCalibrateOffsets
int32_t encode(const ReplyCalibrateOffsets& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // brc
  retCode = encode(message.getBrc(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_CALIBRATE_OFFSETS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyCalibrateOffsets& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // brc
  {
    BoardReturnCode b;
    retCode = decode(b, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setBrc(b);
  }

  return bytes;
}

// GetFault
int32_t encode(const GetFault& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_FAULT, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetFault& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyFault
int32_t encode(const ReplyFault& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // fault
  retCode = encode(message.getFault(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_FAULT, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyFault& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // fault
  {
    uint32_t f;
    retCode = decode(f, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setFault(f);
  }

  return bytes;
}

// ClearFault
int32_t encode(const ClearFault& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // brc
  retCode = encode(message.getBrc(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__CLEAR_FAULT, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ClearFault& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // brc
  {
    BoardReturnCode b;
    retCode = decode(b, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setBrc(b);
  }

  return bytes;
}

// ReplyClearFault
int32_t encode(const ReplyClearFault& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // fault
  retCode = encode(message.getFault(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_CLEAR_FAULT, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyClearFault& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // fault
  {
    uint32_t f;
    retCode = decode(f, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setFault(f);
  }

  return bytes;
}

// SetSampleStreamPolicy
int32_t encode(const SetSampleStreamPolicy& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // policy
  retCode = encode(message.getPolicy(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // rate
  retCode = encode(message.getRate(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SET_SAMPLE_STREAM_POLICY, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SetSampleStreamPolicy& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // policy
  {
    SampleStreamPolicy p;
    retCode = decode(p, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setPolicy(p);
  }

  // rate
  {
    uint32_t r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRate(r);
  }

  return bytes;
}

// ReplySetSampleStreamPolicy
int32_t encode(const ReplySetSampleStreamPolicy& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // brc
  retCode = encode(message.getBrc(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_SET_SAMPLE_STREAM_POLICY, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplySetSampleStreamPolicy& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // brc
  {
    BoardReturnCode b;
    retCode = decode(b, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setBrc(b);
  }

  return bytes;
}

// GetSampleStreamPolicy
int32_t encode(const GetSampleStreamPolicy& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_SAMPLE_STREAM_POLICY, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetSampleStreamPolicy& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyGetSampleStreamPolicy
int32_t encode(const ReplyGetSampleStreamPolicy& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // policy
  retCode = encode(message.getPolicy(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_GET_SAMPLE_STREAM_POLICY, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyGetSampleStreamPolicy& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // policy
  {
    SampleStreamPolicy p;
    retCode = decode(p, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setPolicy(p);
  }

  return bytes;
}

// SetTempFactors
int32_t encode(const SetTempFactors& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // t0
  retCode = encode(message.getT0(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // t1
  retCode = encode(message.getT1(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // t2
  retCode = encode(message.getT2(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // t3
  retCode = encode(message.getT3(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // t4
  retCode = encode(message.getT4(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // t5
  retCode = encode(message.getT5(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SET_TEMP_FACTORS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SetTempFactors& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // t0
  {
    uint32_t t;
    retCode = decode(t, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setT0(t);
  }

  // t1
  {
    uint32_t t;
    retCode = decode(t, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setT1(t);
  }

  // t2
  {
    uint32_t t;
    retCode = decode(t, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setT2(t);
  }

  // t3
  {
    uint32_t t;
    retCode = decode(t, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setT3(t);
  }

  // t4
  {
    uint32_t t;
    retCode = decode(t, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setT4(t);
  }

  // t5
  {
    uint32_t t;
    retCode = decode(t, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setT5(t);
  }

  return bytes;
}

// ReplySetTempFactors
int32_t encode(const ReplySetTempFactors& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // policy
  retCode = encode(message.getPolicy(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_SET_TEMP_FACTORS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplySetTempFactors& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // policy
  {
    SampleStreamPolicy p;
    retCode = decode(p, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setPolicy(p);
  }

  return bytes;
}

// GetCalibrationTemp
int32_t encode(const GetCalibrationTemp& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_CALIBRATION_TEMP, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetCalibrationTemp& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyGetCalibrationTemp
int32_t encode(const ReplyGetCalibrationTemp& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // calibrationTemp
  retCode = encode(message.getCalibrationTemp(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_GET_CALIBRATION_TEMP, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyGetCalibrationTemp& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // calibrationTemp
  {
    int32_t c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setCalibrationTemp(c);
  }

  return bytes;
}

// SetCalibrationMatrixRow
int32_t encode(const SetCalibrationMatrixRow& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // row
  retCode = encode(message.getRow(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c0
  retCode = encode(message.getC0(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c1
  retCode = encode(message.getC1(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c2
  retCode = encode(message.getC2(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c3
  retCode = encode(message.getC3(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c4
  retCode = encode(message.getC4(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c5
  retCode = encode(message.getC5(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SET_CALIBRATION_MATRIX_ROW, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SetCalibrationMatrixRow& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // row
  {
    uint32_t r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRow(r);
  }

  // c0
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC0(c);
  }

  // c1
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC1(c);
  }

  // c2
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC2(c);
  }

  // c3
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC3(c);
  }

  // c4
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC4(c);
  }

  // c5
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC5(c);
  }

  return bytes;
}

// ReplySetCalibrationMatrixRow
int32_t encode(const ReplySetCalibrationMatrixRow& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // brc
  retCode = encode(message.getBrc(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_SET_CALIBRATION_MATRIX_ROW, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplySetCalibrationMatrixRow& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // brc
  {
    BoardReturnCode b;
    retCode = decode(b, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setBrc(b);
  }

  return bytes;
}

// GetCalibrationMatrixRow
int32_t encode(const GetCalibrationMatrixRow& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // row
  retCode = encode(message.getRow(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_CALIBRATION_MATRIX_ROW, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetCalibrationMatrixRow& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // row
  {
    uint32_t r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRow(r);
  }

  return bytes;
}

// ReplyGetCalibrationMatrixRow
int32_t encode(const ReplyGetCalibrationMatrixRow& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // row
  retCode = encode(message.getRow(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c0
  retCode = encode(message.getC0(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c1
  retCode = encode(message.getC1(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c2
  retCode = encode(message.getC2(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c3
  retCode = encode(message.getC3(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c4
  retCode = encode(message.getC4(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c5
  retCode = encode(message.getC5(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_GET_CALIBRATION_MATRIX_ROW, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyGetCalibrationMatrixRow& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // row
  {
    uint32_t r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRow(r);
  }

  // c0
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC0(c);
  }

  // c1
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC1(c);
  }

  // c2
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC2(c);
  }

  // c3
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC3(c);
  }

  // c4
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC4(c);
  }

  // c5
  {
    float c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC5(c);
  }

  return bytes;
}

// GetCalibrationOffsets
int32_t encode(const GetCalibrationOffsets& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_CALIBRATION_OFFSETS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetCalibrationOffsets& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyGetCalibrationOffsets
int32_t encode(const ReplyGetCalibrationOffsets& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c0
  retCode = encode(message.getC0(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c1
  retCode = encode(message.getC1(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c2
  retCode = encode(message.getC2(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c3
  retCode = encode(message.getC3(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c4
  retCode = encode(message.getC4(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // c5
  retCode = encode(message.getC5(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_GET_CALIBRATION_OFFSETS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyGetCalibrationOffsets& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // c0
  {
    uint32_t c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC0(c);
  }

  // c1
  {
    uint32_t c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC1(c);
  }

  // c2
  {
    uint32_t c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC2(c);
  }

  // c3
  {
    uint32_t c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC3(c);
  }

  // c4
  {
    uint32_t c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC4(c);
  }

  // c5
  {
    uint32_t c;
    retCode = decode(c, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setC5(c);
  }

  return bytes;
}

// BCastSensorData
int32_t encode(const BCastSensorData& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Fx
  retCode = encode(message.getFx(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Fy
  retCode = encode(message.getFy(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Fz
  retCode = encode(message.getFz(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Tx
  retCode = encode(message.getTx(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Ty
  retCode = encode(message.getTy(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Tz
  retCode = encode(message.getTz(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // raw0
  retCode = encode(message.getRaw0(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // raw1
  retCode = encode(message.getRaw1(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // raw2
  retCode = encode(message.getRaw2(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // raw3
  retCode = encode(message.getRaw3(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // raw4
  retCode = encode(message.getRaw4(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // raw5
  retCode = encode(message.getRaw5(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__B_CAST_SENSOR_DATA, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(BCastSensorData& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // Fx
  {
    float F;
    retCode = decode(F, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setFx(F);
  }

  // Fy
  {
    float F;
    retCode = decode(F, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setFy(F);
  }

  // Fz
  {
    float F;
    retCode = decode(F, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setFz(F);
  }

  // Tx
  {
    float T;
    retCode = decode(T, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setTx(T);
  }

  // Ty
  {
    float T;
    retCode = decode(T, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setTy(T);
  }

  // Tz
  {
    float T;
    retCode = decode(T, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setTz(T);
  }

  // raw0
  {
    float r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRaw0(r);
  }

  // raw1
  {
    float r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRaw1(r);
  }

  // raw2
  {
    float r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRaw2(r);
  }

  // raw3
  {
    float r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRaw3(r);
  }

  // raw4
  {
    float r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRaw4(r);
  }

  // raw5
  {
    float r;
    retCode = decode(r, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setRaw5(r);
  }

  return bytes;
}

// SaveParamsOnFlash
int32_t encode(const SaveParamsOnFlash& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SAVE_PARAMS_ON_FLASH, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SaveParamsOnFlash& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// SetIpAddress
int32_t encode(const SetIpAddress& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // ipAddress
  retCode = encode(message.getIpAddress(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SET_IP_ADDRESS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SetIpAddress& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // ipAddress
  {
    uint32_t i;
    retCode = decode(i, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setIpAddress(i);
  }

  return bytes;
}

// GetIpAddress
int32_t encode(const GetIpAddress& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_IP_ADDRESS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetIpAddress& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyGetIpAddress
int32_t encode(const ReplyGetIpAddress& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // ipAddress
  retCode = encode(message.getIpAddress(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_GET_IP_ADDRESS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyGetIpAddress& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // ipAddress
  {
    uint32_t i;
    retCode = decode(i, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setIpAddress(i);
  }

  return bytes;
}

// SetMacAddress
int32_t encode(const SetMacAddress& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m0
  retCode = encode(message.getM0(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m1
  retCode = encode(message.getM1(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m2
  retCode = encode(message.getM2(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m3
  retCode = encode(message.getM3(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m4
  retCode = encode(message.getM4(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m5
  retCode = encode(message.getM5(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m6
  retCode = encode(message.getM6(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m7
  retCode = encode(message.getM7(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SET_MAC_ADDRESS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SetMacAddress& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // m0
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM0(m);
  }

  // m1
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM1(m);
  }

  // m2
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM2(m);
  }

  // m3
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM3(m);
  }

  // m4
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM4(m);
  }

  // m5
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM5(m);
  }

  // m6
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM6(m);
  }

  // m7
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM7(m);
  }

  return bytes;
}

// ReplySetMacAddress
int32_t encode(const ReplySetMacAddress& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // macrc
  retCode = encode(message.getMacrc(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_SET_MAC_ADDRESS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplySetMacAddress& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // macrc
  {
    MacAddressReturnCode m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setMacrc(m);
  }

  return bytes;
}

// GetMacAddress
int32_t encode(const GetMacAddress& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_MAC_ADDRESS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetMacAddress& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyGetMacAddress
int32_t encode(const ReplyGetMacAddress& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m0
  retCode = encode(message.getM0(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m1
  retCode = encode(message.getM1(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m2
  retCode = encode(message.getM2(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m3
  retCode = encode(message.getM3(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m4
  retCode = encode(message.getM4(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m5
  retCode = encode(message.getM5(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m6
  retCode = encode(message.getM6(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // m7
  retCode = encode(message.getM7(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_GET_MAC_ADDRESS, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyGetMacAddress& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // m0
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM0(m);
  }

  // m1
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM1(m);
  }

  // m2
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM2(m);
  }

  // m3
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM3(m);
  }

  // m4
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM4(m);
  }

  // m5
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM5(m);
  }

  // m6
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM6(m);
  }

  // m7
  {
    uint8_t m;
    retCode = decode(m, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setM7(m);
  }

  return bytes;
}

// SetNetmask
int32_t encode(const SetNetmask& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // netmask
  retCode = encode(message.getNetmask(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SET_NETMASK, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SetNetmask& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // netmask
  {
    uint32_t n;
    retCode = decode(n, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setNetmask(n);
  }

  return bytes;
}

// GetNetmask
int32_t encode(const GetNetmask& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_NETMASK, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetNetmask& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyGetNetmask
int32_t encode(const ReplyGetNetmask& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // netmask
  retCode = encode(message.getNetmask(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_GET_NETMASK, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyGetNetmask& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // netmask
  {
    uint32_t n;
    retCode = decode(n, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setNetmask(n);
  }

  return bytes;
}

// SetGateway
int32_t encode(const SetGateway& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // gateway
  retCode = encode(message.getGateway(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SET_GATEWAY, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SetGateway& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // gateway
  {
    uint32_t g;
    retCode = decode(g, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setGateway(g);
  }

  return bytes;
}

// GetGateway
int32_t encode(const GetGateway& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__GET_GATEWAY, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(GetGateway& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyGetGateway
int32_t encode(const ReplyGetGateway& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // gateway
  retCode = encode(message.getGateway(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_GET_GATEWAY, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyGetGateway& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // gateway
  {
    uint32_t g;
    retCode = decode(g, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setGateway(g);
  }

  return bytes;
}

// SaveInventoryDataOnFlash
int32_t encode(const SaveInventoryDataOnFlash& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // designCode
  retCode = encode(message.getDesignCode(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // boardVer
  retCode = encode(message.getBoardVer(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // boardRev
  retCode = encode(message.getBoardRev(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // serialNumber
  retCode = encode(message.getSerialNumber(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // dateTime
  retCode = encode(message.getDateTime(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__SAVE_INVENTORY_DATA_ON_FLASH, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(SaveInventoryDataOnFlash& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // designCode
  {
    uint32_t d;
    retCode = decode(d, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setDesignCode(d);
  }

  // boardVer
  {
    uint32_t b;
    retCode = decode(b, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setBoardVer(b);
  }

  // boardRev
  {
    uint32_t b;
    retCode = decode(b, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setBoardRev(b);
  }

  // serialNumber
  {
    uint32_t s;
    retCode = decode(s, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setSerialNumber(s);
  }

  // dateTime
  {
    uint32_t d;
    retCode = decode(d, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setDateTime(d);
  }

  return bytes;
}

// LoadInventoryDataFromFlash
int32_t encode(const LoadInventoryDataFromFlash& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__LOAD_INVENTORY_DATA_FROM_FLASH, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(LoadInventoryDataFromFlash& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  return bytes;
}

// ReplyLoadInventoryDataFromFlash
int32_t encode(const ReplyLoadInventoryDataFromFlash& message, uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  retCode = encode(message.getHeader(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // designCode
  retCode = encode(message.getDesignCode(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // boardVer
  retCode = encode(message.getBoardVer(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // boardRev
  retCode = encode(message.getBoardRev(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // serialNumber
  retCode = encode(message.getSerialNumber(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // dateTime
  retCode = encode(message.getDateTime(), &(binary[bytes]), (size_t)(maxSize - bytes));
  if(retCode < 0)
    return retCode;
  bytes += retCode;

  // Automatically fill the header informations
  packHeaderInfo(MESSAGE_ID__REPLY_LOAD_INVENTORY_DATA_FROM_FLASH, bytes, ENCODING_TYPE, binary);

  return bytes;
}

int32_t decode(ReplyLoadInventoryDataFromFlash& message, const uint8_t* binary, const size_t maxSize)
{
  int32_t bytes = 0;
  int32_t retCode = 0;

  // header
  {
    LIBHeader h;
    retCode = decode(h, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setHeader(h);
  }

  // designCode
  {
    uint32_t d;
    retCode = decode(d, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setDesignCode(d);
  }

  // boardVer
  {
    uint32_t b;
    retCode = decode(b, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setBoardVer(b);
  }

  // boardRev
  {
    uint32_t b;
    retCode = decode(b, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setBoardRev(b);
  }

  // serialNumber
  {
    uint32_t s;
    retCode = decode(s, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setSerialNumber(s);
  }

  // dateTime
  {
    uint32_t d;
    retCode = decode(d, &(binary[bytes]), (size_t)(maxSize - bytes)) ;
    if(retCode < 0)
      return retCode;
    bytes += retCode;

    message.setDateTime(d);
  }

  return bytes;
}

// Enum BoardReturnCode
int32_t encode(const BoardReturnCode& enumerative, uint8_t* binary, const size_t maxSize)
{
  return encode((int32_t)enumerative, binary, maxSize);
}

int32_t decode(BoardReturnCode& enumerative, const uint8_t* binary, const size_t maxSize)
{
  int32_t tempVar = 0;
  int32_t retCode = decode(tempVar, binary, maxSize);
  if(retCode > 0)
    enumerative = (BoardReturnCode)tempVar;
  return retCode;
}

// Enum PROTOCOLVERSION
int32_t encode(const PROTOCOLVERSION& enumerative, uint8_t* binary, const size_t maxSize)
{
  return encode((int32_t)enumerative, binary, maxSize);
}

int32_t decode(PROTOCOLVERSION& enumerative, const uint8_t* binary, const size_t maxSize)
{
  int32_t tempVar = 0;
  int32_t retCode = decode(tempVar, binary, maxSize);
  if(retCode > 0)
    enumerative = (PROTOCOLVERSION)tempVar;
  return retCode;
}

// Enum SampleStreamPolicy
int32_t encode(const SampleStreamPolicy& enumerative, uint8_t* binary, const size_t maxSize)
{
  return encode((int32_t)enumerative, binary, maxSize);
}

int32_t decode(SampleStreamPolicy& enumerative, const uint8_t* binary, const size_t maxSize)
{
  int32_t tempVar = 0;
  int32_t retCode = decode(tempVar, binary, maxSize);
  if(retCode > 0)
    enumerative = (SampleStreamPolicy)tempVar;
  return retCode;
}

// Enum MacAddressReturnCode
int32_t encode(const MacAddressReturnCode& enumerative, uint8_t* binary, const size_t maxSize)
{
  return encode((int32_t)enumerative, binary, maxSize);
}

int32_t decode(MacAddressReturnCode& enumerative, const uint8_t* binary, const size_t maxSize)
{
  int32_t tempVar = 0;
  int32_t retCode = decode(tempVar, binary, maxSize);
  if(retCode > 0)
    enumerative = (MacAddressReturnCode)tempVar;
  return retCode;
}


} // namespace Multitorque

