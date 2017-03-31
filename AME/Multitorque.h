#ifndef _MULTITORQUE_H_
#define _MULTITORQUE_H_

#include <stdint.h>
#include <string.h>

// Auto-fill definitions
#define AUTOFILL_MULTITORQUE_HEADER
#define AUTOFILL_MULTITORQUE_HEADER_ID
#define AUTOFILL_MULTITORQUE_HEADER_SIZE
#define AUTOFILL_MULTITORQUE_HEADER_ENCODING

namespace Multitorque
{

// Message IDs definitions
enum {
  MESSAGE_ID__GET_BOARD_INFO = 1,
  MESSAGE_ID__REPLY_BOARD_INFO = 2,
  MESSAGE_ID__CALIBRATE_OFFSETS = 3,
  MESSAGE_ID__REPLY_CALIBRATE_OFFSETS = 4,
  MESSAGE_ID__GET_FAULT = 5,
  MESSAGE_ID__REPLY_FAULT = 6,
  MESSAGE_ID__CLEAR_FAULT = 7,
  MESSAGE_ID__REPLY_CLEAR_FAULT = 8,
  MESSAGE_ID__SET_SAMPLE_STREAM_POLICY = 9,
  MESSAGE_ID__REPLY_SET_SAMPLE_STREAM_POLICY = 10,
  MESSAGE_ID__GET_SAMPLE_STREAM_POLICY = 11,
  MESSAGE_ID__REPLY_GET_SAMPLE_STREAM_POLICY = 12,
  MESSAGE_ID__SET_TEMP_FACTORS = 13,
  MESSAGE_ID__REPLY_SET_TEMP_FACTORS = 14,
  MESSAGE_ID__GET_CALIBRATION_TEMP = 15,
  MESSAGE_ID__REPLY_GET_CALIBRATION_TEMP = 16,
  MESSAGE_ID__SET_CALIBRATION_MATRIX_ROW = 17,
  MESSAGE_ID__REPLY_SET_CALIBRATION_MATRIX_ROW = 18,
  MESSAGE_ID__GET_CALIBRATION_MATRIX_ROW = 19,
  MESSAGE_ID__REPLY_GET_CALIBRATION_MATRIX_ROW = 20,
  MESSAGE_ID__GET_CALIBRATION_OFFSETS = 21,
  MESSAGE_ID__REPLY_GET_CALIBRATION_OFFSETS = 22,
  MESSAGE_ID__B_CAST_SENSOR_DATA = 23,
  MESSAGE_ID__SAVE_PARAMS_ON_FLASH = 24,
  MESSAGE_ID__SET_IP_ADDRESS = 25,
  MESSAGE_ID__GET_IP_ADDRESS = 26,
  MESSAGE_ID__REPLY_GET_IP_ADDRESS = 27,
  MESSAGE_ID__SET_MAC_ADDRESS = 28,
  MESSAGE_ID__REPLY_SET_MAC_ADDRESS = 29,
  MESSAGE_ID__GET_MAC_ADDRESS = 30,
  MESSAGE_ID__REPLY_GET_MAC_ADDRESS = 31,
  MESSAGE_ID__SET_NETMASK = 32,
  MESSAGE_ID__GET_NETMASK = 33,
  MESSAGE_ID__REPLY_GET_NETMASK = 34,
  MESSAGE_ID__SET_GATEWAY = 35,
  MESSAGE_ID__GET_GATEWAY = 36,
  MESSAGE_ID__REPLY_GET_GATEWAY = 37,
  MESSAGE_ID__SAVE_INVENTORY_DATA_ON_FLASH = 38,
  MESSAGE_ID__LOAD_INVENTORY_DATA_FROM_FLASH = 39,
  MESSAGE_ID__REPLY_LOAD_INVENTORY_DATA_FROM_FLASH = 40,
};

// Enum definitions
enum BoardReturnCode {
  BoardReturnCode_OK = 0,
  BoardReturnCode_ERR = 1,
};

enum PROTOCOLVERSION {
  PROTOCOLVERSION_RELEASE = 1,
};

enum SampleStreamPolicy {
  SampleStreamPolicy_OFF = 0,
  SampleStreamPolicy_NORMAL = 1,
};

enum MacAddressReturnCode {
  MacAddressReturnCode_OK = 0,
  MacAddressReturnCode_ERR = 1,
};

/**************************************************/
class LIBHeader
{
  public:
    LIBHeader(uint16_t _id=0) : id(_id) ,
      size(),
      encoding()
    { }

    // Get methods.
    uint16_t getId() const     { return id;  }
    uint16_t getSize() const     { return size;  }
    uint8_t getEncoding() const     { return encoding;  }

    // Set methods.
    void setId(const uint16_t& _i)    { id = _i;  }
    void setSize(const uint16_t& _s)    { size = _s;  }
    void setEncoding(const uint8_t& _e)    { encoding = _e;  }

  private:
    uint16_t id;
    uint16_t size;
    uint8_t encoding;
};

/**************************************************/
class GetBoardInfo
{
  public:
    GetBoardInfo() : header(MESSAGE_ID__GET_BOARD_INFO) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyBoardInfo
{
  public:
    ReplyBoardInfo() : header(MESSAGE_ID__REPLY_BOARD_INFO) ,
      release()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getRelease() const     { return release;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setRelease(const uint32_t& _r)    { release = _r;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t release;
};

/**************************************************/
class CalibrateOffsets
{
  public:
    CalibrateOffsets() : header(MESSAGE_ID__CALIBRATE_OFFSETS) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyCalibrateOffsets
{
  public:
    ReplyCalibrateOffsets() : header(MESSAGE_ID__REPLY_CALIBRATE_OFFSETS) ,
      brc()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    BoardReturnCode getBrc() const     { return brc;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setBrc(const BoardReturnCode& _b)    { brc = _b;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    BoardReturnCode brc;
};

/**************************************************/
class GetFault
{
  public:
    GetFault() : header(MESSAGE_ID__GET_FAULT) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyFault
{
  public:
    ReplyFault() : header(MESSAGE_ID__REPLY_FAULT) ,
      fault()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getFault() const     { return fault;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setFault(const uint32_t& _f)    { fault = _f;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t fault;
};

/**************************************************/
class ClearFault
{
  public:
    ClearFault() : header(MESSAGE_ID__CLEAR_FAULT) ,
      brc()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    BoardReturnCode getBrc() const     { return brc;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setBrc(const BoardReturnCode& _b)    { brc = _b;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    BoardReturnCode brc;
};

/**************************************************/
class ReplyClearFault
{
  public:
    ReplyClearFault() : header(MESSAGE_ID__REPLY_CLEAR_FAULT) ,
      fault()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getFault() const     { return fault;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setFault(const uint32_t& _f)    { fault = _f;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t fault;
};

/**************************************************/
class SetSampleStreamPolicy
{
  public:
    SetSampleStreamPolicy() : header(MESSAGE_ID__SET_SAMPLE_STREAM_POLICY) ,
      policy(),
      rate()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    SampleStreamPolicy getPolicy() const     { return policy;  }
    uint32_t getRate() const     { return rate;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setPolicy(const SampleStreamPolicy& _p)    { policy = _p;  }
    void setRate(const uint32_t& _r)    { rate = _r;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    SampleStreamPolicy policy;
    uint32_t rate;
};

/**************************************************/
class ReplySetSampleStreamPolicy
{
  public:
    ReplySetSampleStreamPolicy() : header(MESSAGE_ID__REPLY_SET_SAMPLE_STREAM_POLICY) ,
      brc()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    BoardReturnCode getBrc() const     { return brc;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setBrc(const BoardReturnCode& _b)    { brc = _b;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    BoardReturnCode brc;
};

/**************************************************/
class GetSampleStreamPolicy
{
  public:
    GetSampleStreamPolicy() : header(MESSAGE_ID__GET_SAMPLE_STREAM_POLICY) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyGetSampleStreamPolicy
{
  public:
    ReplyGetSampleStreamPolicy() : header(MESSAGE_ID__REPLY_GET_SAMPLE_STREAM_POLICY) ,
      policy()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    SampleStreamPolicy getPolicy() const     { return policy;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setPolicy(const SampleStreamPolicy& _p)    { policy = _p;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    SampleStreamPolicy policy;
};

/**************************************************/
class SetTempFactors
{
  public:
    SetTempFactors() : header(MESSAGE_ID__SET_TEMP_FACTORS) ,
      t0(),
      t1(),
      t2(),
      t3(),
      t4(),
      t5()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getT0() const     { return t0;  }
    uint32_t getT1() const     { return t1;  }
    uint32_t getT2() const     { return t2;  }
    uint32_t getT3() const     { return t3;  }
    uint32_t getT4() const     { return t4;  }
    uint32_t getT5() const     { return t5;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setT0(const uint32_t& _t)    { t0 = _t;  }
    void setT1(const uint32_t& _t)    { t1 = _t;  }
    void setT2(const uint32_t& _t)    { t2 = _t;  }
    void setT3(const uint32_t& _t)    { t3 = _t;  }
    void setT4(const uint32_t& _t)    { t4 = _t;  }
    void setT5(const uint32_t& _t)    { t5 = _t;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
};

/**************************************************/
class ReplySetTempFactors
{
  public:
    ReplySetTempFactors() : header(MESSAGE_ID__REPLY_SET_TEMP_FACTORS) ,
      policy()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    SampleStreamPolicy getPolicy() const     { return policy;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setPolicy(const SampleStreamPolicy& _p)    { policy = _p;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    SampleStreamPolicy policy;
};

/**************************************************/
class GetCalibrationTemp
{
  public:
    GetCalibrationTemp() : header(MESSAGE_ID__GET_CALIBRATION_TEMP) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyGetCalibrationTemp
{
  public:
    ReplyGetCalibrationTemp() : header(MESSAGE_ID__REPLY_GET_CALIBRATION_TEMP) ,
      calibrationTemp()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    int32_t getCalibrationTemp() const     { return calibrationTemp;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setCalibrationTemp(const int32_t& _c)    { calibrationTemp = _c;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    int32_t calibrationTemp;
};

/**************************************************/
class SetCalibrationMatrixRow
{
  public:
    SetCalibrationMatrixRow() : header(MESSAGE_ID__SET_CALIBRATION_MATRIX_ROW) ,
      row(),
      c0(),
      c1(),
      c2(),
      c3(),
      c4(),
      c5()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getRow() const     { return row;  }
    float getC0() const     { return c0;  }
    float getC1() const     { return c1;  }
    float getC2() const     { return c2;  }
    float getC3() const     { return c3;  }
    float getC4() const     { return c4;  }
    float getC5() const     { return c5;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setRow(const uint32_t& _r)    { row = _r;  }
    void setC0(const float& _c)    { c0 = _c;  }
    void setC1(const float& _c)    { c1 = _c;  }
    void setC2(const float& _c)    { c2 = _c;  }
    void setC3(const float& _c)    { c3 = _c;  }
    void setC4(const float& _c)    { c4 = _c;  }
    void setC5(const float& _c)    { c5 = _c;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t row;
    float c0;
    float c1;
    float c2;
    float c3;
    float c4;
    float c5;
};

/**************************************************/
class ReplySetCalibrationMatrixRow
{
  public:
    ReplySetCalibrationMatrixRow() : header(MESSAGE_ID__REPLY_SET_CALIBRATION_MATRIX_ROW) ,
      brc()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    BoardReturnCode getBrc() const     { return brc;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setBrc(const BoardReturnCode& _b)    { brc = _b;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    BoardReturnCode brc;
};

/**************************************************/
class GetCalibrationMatrixRow
{
  public:
    GetCalibrationMatrixRow() : header(MESSAGE_ID__GET_CALIBRATION_MATRIX_ROW) ,
      row()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getRow() const     { return row;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setRow(const uint32_t& _r)    { row = _r;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t row;
};

/**************************************************/
class ReplyGetCalibrationMatrixRow
{
  public:
    ReplyGetCalibrationMatrixRow() : header(MESSAGE_ID__REPLY_GET_CALIBRATION_MATRIX_ROW) ,
      row(),
      c0(),
      c1(),
      c2(),
      c3(),
      c4(),
      c5()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getRow() const     { return row;  }
    float getC0() const     { return c0;  }
    float getC1() const     { return c1;  }
    float getC2() const     { return c2;  }
    float getC3() const     { return c3;  }
    float getC4() const     { return c4;  }
    float getC5() const     { return c5;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setRow(const uint32_t& _r)    { row = _r;  }
    void setC0(const float& _c)    { c0 = _c;  }
    void setC1(const float& _c)    { c1 = _c;  }
    void setC2(const float& _c)    { c2 = _c;  }
    void setC3(const float& _c)    { c3 = _c;  }
    void setC4(const float& _c)    { c4 = _c;  }
    void setC5(const float& _c)    { c5 = _c;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t row;
    float c0;
    float c1;
    float c2;
    float c3;
    float c4;
    float c5;
};

/**************************************************/
class GetCalibrationOffsets
{
  public:
    GetCalibrationOffsets() : header(MESSAGE_ID__GET_CALIBRATION_OFFSETS) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyGetCalibrationOffsets
{
  public:
    ReplyGetCalibrationOffsets() : header(MESSAGE_ID__REPLY_GET_CALIBRATION_OFFSETS) ,
      c0(),
      c1(),
      c2(),
      c3(),
      c4(),
      c5()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getC0() const     { return c0;  }
    uint32_t getC1() const     { return c1;  }
    uint32_t getC2() const     { return c2;  }
    uint32_t getC3() const     { return c3;  }
    uint32_t getC4() const     { return c4;  }
    uint32_t getC5() const     { return c5;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setC0(const uint32_t& _c)    { c0 = _c;  }
    void setC1(const uint32_t& _c)    { c1 = _c;  }
    void setC2(const uint32_t& _c)    { c2 = _c;  }
    void setC3(const uint32_t& _c)    { c3 = _c;  }
    void setC4(const uint32_t& _c)    { c4 = _c;  }
    void setC5(const uint32_t& _c)    { c5 = _c;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t c0;
    uint32_t c1;
    uint32_t c2;
    uint32_t c3;
    uint32_t c4;
    uint32_t c5;
};

/**************************************************/
class BCastSensorData
{
  public:
    BCastSensorData() : header(MESSAGE_ID__B_CAST_SENSOR_DATA) ,
      Fx(),
      Fy(),
      Fz(),
      Tx(),
      Ty(),
      Tz(),
      raw0(),
      raw1(),
      raw2(),
      raw3(),
      raw4(),
      raw5()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    float getFx() const     { return Fx;  }
    float getFy() const     { return Fy;  }
    float getFz() const     { return Fz;  }
    float getTx() const     { return Tx;  }
    float getTy() const     { return Ty;  }
    float getTz() const     { return Tz;  }
    float getRaw0() const     { return raw0;  }
    float getRaw1() const     { return raw1;  }
    float getRaw2() const     { return raw2;  }
    float getRaw3() const     { return raw3;  }
    float getRaw4() const     { return raw4;  }
    float getRaw5() const     { return raw5;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setFx(const float& _F)    { Fx = _F;  }
    void setFy(const float& _F)    { Fy = _F;  }
    void setFz(const float& _F)    { Fz = _F;  }
    void setTx(const float& _T)    { Tx = _T;  }
    void setTy(const float& _T)    { Ty = _T;  }
    void setTz(const float& _T)    { Tz = _T;  }
    void setRaw0(const float& _r)    { raw0 = _r;  }
    void setRaw1(const float& _r)    { raw1 = _r;  }
    void setRaw2(const float& _r)    { raw2 = _r;  }
    void setRaw3(const float& _r)    { raw3 = _r;  }
    void setRaw4(const float& _r)    { raw4 = _r;  }
    void setRaw5(const float& _r)    { raw5 = _r;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    float Fx;
    float Fy;
    float Fz;
    float Tx;
    float Ty;
    float Tz;
    float raw0;
    float raw1;
    float raw2;
    float raw3;
    float raw4;
    float raw5;
};

/**************************************************/
class SaveParamsOnFlash
{
  public:
    SaveParamsOnFlash() : header(MESSAGE_ID__SAVE_PARAMS_ON_FLASH) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class SetIpAddress
{
  public:
    SetIpAddress() : header(MESSAGE_ID__SET_IP_ADDRESS) ,
      ipAddress()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getIpAddress() const     { return ipAddress;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setIpAddress(const uint32_t& _i)    { ipAddress = _i;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t ipAddress;
};

/**************************************************/
class GetIpAddress
{
  public:
    GetIpAddress() : header(MESSAGE_ID__GET_IP_ADDRESS) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyGetIpAddress
{
  public:
    ReplyGetIpAddress() : header(MESSAGE_ID__REPLY_GET_IP_ADDRESS) ,
      ipAddress()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getIpAddress() const     { return ipAddress;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setIpAddress(const uint32_t& _i)    { ipAddress = _i;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t ipAddress;
};

/**************************************************/
class SetMacAddress
{
  public:
    SetMacAddress() : header(MESSAGE_ID__SET_MAC_ADDRESS) ,
      m0(),
      m1(),
      m2(),
      m3(),
      m4(),
      m5(),
      m6(),
      m7()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint8_t getM0() const     { return m0;  }
    uint8_t getM1() const     { return m1;  }
    uint8_t getM2() const     { return m2;  }
    uint8_t getM3() const     { return m3;  }
    uint8_t getM4() const     { return m4;  }
    uint8_t getM5() const     { return m5;  }
    uint8_t getM6() const     { return m6;  }
    uint8_t getM7() const     { return m7;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setM0(const uint8_t& _m)    { m0 = _m;  }
    void setM1(const uint8_t& _m)    { m1 = _m;  }
    void setM2(const uint8_t& _m)    { m2 = _m;  }
    void setM3(const uint8_t& _m)    { m3 = _m;  }
    void setM4(const uint8_t& _m)    { m4 = _m;  }
    void setM5(const uint8_t& _m)    { m5 = _m;  }
    void setM6(const uint8_t& _m)    { m6 = _m;  }
    void setM7(const uint8_t& _m)    { m7 = _m;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint8_t m0;
    uint8_t m1;
    uint8_t m2;
    uint8_t m3;
    uint8_t m4;
    uint8_t m5;
    uint8_t m6;
    uint8_t m7;
};

/**************************************************/
class ReplySetMacAddress
{
  public:
    ReplySetMacAddress() : header(MESSAGE_ID__REPLY_SET_MAC_ADDRESS) ,
      macrc()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    MacAddressReturnCode getMacrc() const     { return macrc;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setMacrc(const MacAddressReturnCode& _m)    { macrc = _m;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    MacAddressReturnCode macrc;
};

/**************************************************/
class GetMacAddress
{
  public:
    GetMacAddress() : header(MESSAGE_ID__GET_MAC_ADDRESS) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyGetMacAddress
{
  public:
    ReplyGetMacAddress() : header(MESSAGE_ID__REPLY_GET_MAC_ADDRESS) ,
      m0(),
      m1(),
      m2(),
      m3(),
      m4(),
      m5(),
      m6(),
      m7()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint8_t getM0() const     { return m0;  }
    uint8_t getM1() const     { return m1;  }
    uint8_t getM2() const     { return m2;  }
    uint8_t getM3() const     { return m3;  }
    uint8_t getM4() const     { return m4;  }
    uint8_t getM5() const     { return m5;  }
    uint8_t getM6() const     { return m6;  }
    uint8_t getM7() const     { return m7;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setM0(const uint8_t& _m)    { m0 = _m;  }
    void setM1(const uint8_t& _m)    { m1 = _m;  }
    void setM2(const uint8_t& _m)    { m2 = _m;  }
    void setM3(const uint8_t& _m)    { m3 = _m;  }
    void setM4(const uint8_t& _m)    { m4 = _m;  }
    void setM5(const uint8_t& _m)    { m5 = _m;  }
    void setM6(const uint8_t& _m)    { m6 = _m;  }
    void setM7(const uint8_t& _m)    { m7 = _m;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint8_t m0;
    uint8_t m1;
    uint8_t m2;
    uint8_t m3;
    uint8_t m4;
    uint8_t m5;
    uint8_t m6;
    uint8_t m7;
};

/**************************************************/
class SetNetmask
{
  public:
    SetNetmask() : header(MESSAGE_ID__SET_NETMASK) ,
      netmask()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getNetmask() const     { return netmask;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setNetmask(const uint32_t& _n)    { netmask = _n;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t netmask;
};

/**************************************************/
class GetNetmask
{
  public:
    GetNetmask() : header(MESSAGE_ID__GET_NETMASK) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyGetNetmask
{
  public:
    ReplyGetNetmask() : header(MESSAGE_ID__REPLY_GET_NETMASK) ,
      netmask()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getNetmask() const     { return netmask;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setNetmask(const uint32_t& _n)    { netmask = _n;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t netmask;
};

/**************************************************/
class SetGateway
{
  public:
    SetGateway() : header(MESSAGE_ID__SET_GATEWAY) ,
      gateway()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getGateway() const     { return gateway;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setGateway(const uint32_t& _g)    { gateway = _g;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t gateway;
};

/**************************************************/
class GetGateway
{
  public:
    GetGateway() : header(MESSAGE_ID__GET_GATEWAY) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyGetGateway
{
  public:
    ReplyGetGateway() : header(MESSAGE_ID__REPLY_GET_GATEWAY) ,
      gateway()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getGateway() const     { return gateway;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setGateway(const uint32_t& _g)    { gateway = _g;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t gateway;
};

/**************************************************/
class SaveInventoryDataOnFlash
{
  public:
    SaveInventoryDataOnFlash() : header(MESSAGE_ID__SAVE_INVENTORY_DATA_ON_FLASH) ,
      designCode(),
      boardVer(),
      boardRev(),
      serialNumber(),
      dateTime()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getDesignCode() const     { return designCode;  }
    uint32_t getBoardVer() const     { return boardVer;  }
    uint32_t getBoardRev() const     { return boardRev;  }
    uint32_t getSerialNumber() const     { return serialNumber;  }
    uint32_t getDateTime() const     { return dateTime;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setDesignCode(const uint32_t& _d)    { designCode = _d;  }
    void setBoardVer(const uint32_t& _b)    { boardVer = _b;  }
    void setBoardRev(const uint32_t& _b)    { boardRev = _b;  }
    void setSerialNumber(const uint32_t& _s)    { serialNumber = _s;  }
    void setDateTime(const uint32_t& _d)    { dateTime = _d;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t designCode;
    uint32_t boardVer;
    uint32_t boardRev;
    uint32_t serialNumber;
    uint32_t dateTime;
};

/**************************************************/
class LoadInventoryDataFromFlash
{
  public:
    LoadInventoryDataFromFlash() : header(MESSAGE_ID__LOAD_INVENTORY_DATA_FROM_FLASH) 
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
};

/**************************************************/
class ReplyLoadInventoryDataFromFlash
{
  public:
    ReplyLoadInventoryDataFromFlash() : header(MESSAGE_ID__REPLY_LOAD_INVENTORY_DATA_FROM_FLASH) ,
      designCode(),
      boardVer(),
      boardRev(),
      serialNumber(),
      dateTime()
    { }

    // Get methods.
    LIBHeader getHeader() const     { return header;  }
    uint32_t getDesignCode() const     { return designCode;  }
    uint32_t getBoardVer() const     { return boardVer;  }
    uint32_t getBoardRev() const     { return boardRev;  }
    uint32_t getSerialNumber() const     { return serialNumber;  }
    uint32_t getDateTime() const     { return dateTime;  }

    // Set methods.
    void setHeader(const LIBHeader& _h)    { header = _h;  }
    void setDesignCode(const uint32_t& _d)    { designCode = _d;  }
    void setBoardVer(const uint32_t& _b)    { boardVer = _b;  }
    void setBoardRev(const uint32_t& _b)    { boardRev = _b;  }
    void setSerialNumber(const uint32_t& _s)    { serialNumber = _s;  }
    void setDateTime(const uint32_t& _d)    { dateTime = _d;  }

    // Header direct access methods
    uint16_t getHeaderId() const     { return header.getId();  }
    uint16_t getHeaderSize() const     { return header.getSize();  }
    uint8_t getHeaderEncoding() const     { return header.getEncoding();  }

  private:
    LIBHeader header;
    uint32_t designCode;
    uint32_t boardVer;
    uint32_t boardRev;
    uint32_t serialNumber;
    uint32_t dateTime;
};


} // namespace Multitorque

#endif  // _MULTITORQUE_H_
