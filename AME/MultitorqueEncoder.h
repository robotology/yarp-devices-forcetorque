#ifndef _MULTITORQUEENCODER_H_
#define _MULTITORQUEENCODER_H_

#include "Multitorque.h"

namespace Multitorque
{

// Header encoding and decoding functions.
int32_t unpackHeaderInfo(LIBHeader& header, const uint8_t* binary, const size_t maxSize);
int32_t encode(const LIBHeader& header, uint8_t* binary, const size_t maxSize);
int32_t decode(LIBHeader& header, const uint8_t* binary, const size_t maxSize);

// Message GetBoardInfo encoding and decoding functions.
int32_t encode(const GetBoardInfo& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetBoardInfo& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyBoardInfo encoding and decoding functions.
int32_t encode(const ReplyBoardInfo& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyBoardInfo& message, const uint8_t* binary, const size_t maxSize);

// Message CalibrateOffsets encoding and decoding functions.
int32_t encode(const CalibrateOffsets& message, uint8_t* binary, const size_t maxSize);
int32_t decode(CalibrateOffsets& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyCalibrateOffsets encoding and decoding functions.
int32_t encode(const ReplyCalibrateOffsets& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyCalibrateOffsets& message, const uint8_t* binary, const size_t maxSize);

// Message GetFault encoding and decoding functions.
int32_t encode(const GetFault& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetFault& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyFault encoding and decoding functions.
int32_t encode(const ReplyFault& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyFault& message, const uint8_t* binary, const size_t maxSize);

// Message ClearFault encoding and decoding functions.
int32_t encode(const ClearFault& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ClearFault& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyClearFault encoding and decoding functions.
int32_t encode(const ReplyClearFault& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyClearFault& message, const uint8_t* binary, const size_t maxSize);

// Message SetSampleStreamPolicy encoding and decoding functions.
int32_t encode(const SetSampleStreamPolicy& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SetSampleStreamPolicy& message, const uint8_t* binary, const size_t maxSize);

// Message ReplySetSampleStreamPolicy encoding and decoding functions.
int32_t encode(const ReplySetSampleStreamPolicy& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplySetSampleStreamPolicy& message, const uint8_t* binary, const size_t maxSize);

// Message GetSampleStreamPolicy encoding and decoding functions.
int32_t encode(const GetSampleStreamPolicy& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetSampleStreamPolicy& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyGetSampleStreamPolicy encoding and decoding functions.
int32_t encode(const ReplyGetSampleStreamPolicy& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyGetSampleStreamPolicy& message, const uint8_t* binary, const size_t maxSize);

// Message SetTempFactors encoding and decoding functions.
int32_t encode(const SetTempFactors& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SetTempFactors& message, const uint8_t* binary, const size_t maxSize);

// Message ReplySetTempFactors encoding and decoding functions.
int32_t encode(const ReplySetTempFactors& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplySetTempFactors& message, const uint8_t* binary, const size_t maxSize);

// Message GetCalibrationTemp encoding and decoding functions.
int32_t encode(const GetCalibrationTemp& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetCalibrationTemp& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyGetCalibrationTemp encoding and decoding functions.
int32_t encode(const ReplyGetCalibrationTemp& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyGetCalibrationTemp& message, const uint8_t* binary, const size_t maxSize);

// Message SetCalibrationMatrixRow encoding and decoding functions.
int32_t encode(const SetCalibrationMatrixRow& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SetCalibrationMatrixRow& message, const uint8_t* binary, const size_t maxSize);

// Message ReplySetCalibrationMatrixRow encoding and decoding functions.
int32_t encode(const ReplySetCalibrationMatrixRow& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplySetCalibrationMatrixRow& message, const uint8_t* binary, const size_t maxSize);

// Message GetCalibrationMatrixRow encoding and decoding functions.
int32_t encode(const GetCalibrationMatrixRow& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetCalibrationMatrixRow& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyGetCalibrationMatrixRow encoding and decoding functions.
int32_t encode(const ReplyGetCalibrationMatrixRow& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyGetCalibrationMatrixRow& message, const uint8_t* binary, const size_t maxSize);

// Message GetCalibrationOffsets encoding and decoding functions.
int32_t encode(const GetCalibrationOffsets& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetCalibrationOffsets& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyGetCalibrationOffsets encoding and decoding functions.
int32_t encode(const ReplyGetCalibrationOffsets& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyGetCalibrationOffsets& message, const uint8_t* binary, const size_t maxSize);

// Message BCastSensorData encoding and decoding functions.
int32_t encode(const BCastSensorData& message, uint8_t* binary, const size_t maxSize);
int32_t decode(BCastSensorData& message, const uint8_t* binary, const size_t maxSize);

// Message SaveParamsOnFlash encoding and decoding functions.
int32_t encode(const SaveParamsOnFlash& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SaveParamsOnFlash& message, const uint8_t* binary, const size_t maxSize);

// Message SetIpAddress encoding and decoding functions.
int32_t encode(const SetIpAddress& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SetIpAddress& message, const uint8_t* binary, const size_t maxSize);

// Message GetIpAddress encoding and decoding functions.
int32_t encode(const GetIpAddress& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetIpAddress& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyGetIpAddress encoding and decoding functions.
int32_t encode(const ReplyGetIpAddress& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyGetIpAddress& message, const uint8_t* binary, const size_t maxSize);

// Message SetMacAddress encoding and decoding functions.
int32_t encode(const SetMacAddress& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SetMacAddress& message, const uint8_t* binary, const size_t maxSize);

// Message ReplySetMacAddress encoding and decoding functions.
int32_t encode(const ReplySetMacAddress& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplySetMacAddress& message, const uint8_t* binary, const size_t maxSize);

// Message GetMacAddress encoding and decoding functions.
int32_t encode(const GetMacAddress& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetMacAddress& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyGetMacAddress encoding and decoding functions.
int32_t encode(const ReplyGetMacAddress& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyGetMacAddress& message, const uint8_t* binary, const size_t maxSize);

// Message SetNetmask encoding and decoding functions.
int32_t encode(const SetNetmask& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SetNetmask& message, const uint8_t* binary, const size_t maxSize);

// Message GetNetmask encoding and decoding functions.
int32_t encode(const GetNetmask& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetNetmask& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyGetNetmask encoding and decoding functions.
int32_t encode(const ReplyGetNetmask& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyGetNetmask& message, const uint8_t* binary, const size_t maxSize);

// Message SetGateway encoding and decoding functions.
int32_t encode(const SetGateway& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SetGateway& message, const uint8_t* binary, const size_t maxSize);

// Message GetGateway encoding and decoding functions.
int32_t encode(const GetGateway& message, uint8_t* binary, const size_t maxSize);
int32_t decode(GetGateway& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyGetGateway encoding and decoding functions.
int32_t encode(const ReplyGetGateway& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyGetGateway& message, const uint8_t* binary, const size_t maxSize);

// Message SaveInventoryDataOnFlash encoding and decoding functions.
int32_t encode(const SaveInventoryDataOnFlash& message, uint8_t* binary, const size_t maxSize);
int32_t decode(SaveInventoryDataOnFlash& message, const uint8_t* binary, const size_t maxSize);

// Message LoadInventoryDataFromFlash encoding and decoding functions.
int32_t encode(const LoadInventoryDataFromFlash& message, uint8_t* binary, const size_t maxSize);
int32_t decode(LoadInventoryDataFromFlash& message, const uint8_t* binary, const size_t maxSize);

// Message ReplyLoadInventoryDataFromFlash encoding and decoding functions.
int32_t encode(const ReplyLoadInventoryDataFromFlash& message, uint8_t* binary, const size_t maxSize);
int32_t decode(ReplyLoadInventoryDataFromFlash& message, const uint8_t* binary, const size_t maxSize);

// Enums encoding
int32_t encode(const BoardReturnCode& enumerative, uint8_t* binary, const size_t maxSize);
int32_t decode(BoardReturnCode& enumerative, const uint8_t* binary, const size_t maxSize);

int32_t encode(const PROTOCOLVERSION& enumerative, uint8_t* binary, const size_t maxSize);
int32_t decode(PROTOCOLVERSION& enumerative, const uint8_t* binary, const size_t maxSize);

int32_t encode(const SampleStreamPolicy& enumerative, uint8_t* binary, const size_t maxSize);
int32_t decode(SampleStreamPolicy& enumerative, const uint8_t* binary, const size_t maxSize);

int32_t encode(const MacAddressReturnCode& enumerative, uint8_t* binary, const size_t maxSize);
int32_t decode(MacAddressReturnCode& enumerative, const uint8_t* binary, const size_t maxSize);


} // namespace Multitorque

#endif  // _MULTITORQUEENCODER_H_
