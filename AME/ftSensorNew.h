/**
 * @file
 * @author  Giuseppe Iellamo <giuseppe.iellamo@iit.it>
 * @author  Igor Ingegnosi <igor.ingegnosi@iit.it>
 * @version 1.0
 *
 * @copyright (c) IIT Fondazione Istituto Italiano
 *            di Tecnologia. All rights reserved
 *
 */

#ifndef FTSENSORNEW_H_
#define FTSENSORNEW_H_

#include <string>
#include <thread>
#include <mutex>
#include "Multitorque.h"
#include "ethService.h"
#include "rehab.h"
#include "osutil.h"

using namespace std;

namespace rehab
{
using Ethservice::MacAddress;
/**
 * @brief The FtSensorNew class. Wraps the communication with
 *        the Sensor with LIB based firmware
 */
class FtSensorNew final
{
public:

  FtSensorNew(Ethservice::EthInterfaceManager* bdm_, board_id_t bdid_);
  ~FtSensorNew() ;

  /* Constants definitions */
  static constexpr const size_t FT_SENSOR_AXIS   = 6;

  /* Type definitions */
  ///Array of float which contains force and torque for every three axis
  using FTSensorData = std::array< float, FT_SENSOR_AXIS > ;
  ///Array of float which contains all calibration data
  using FTCalibrationOffsets = std::array< uint32_t, FT_SENSOR_AXIS > ;

  int32_t getFTData (FTSensorData &ftData);
  int32_t getRawFTData (FTSensorData &rawFTData);
  void setSampleStreamPol(Multitorque::SampleStreamPolicy pol, uint32_t rate);
  int32_t calibrateOffsets() ;
  void getCalibrationOffsets(FTCalibrationOffsets &calibrationOffsets);

  void setMtxRow(uint32_t row, FTSensorData &data);
  void getMtxRow(uint32_t row, FTSensorData &mtxRow);
  void saveParamsOnFlash();
  uint32_t getBoardInfo();

  bool isConnectivityOk();


  // Network parameters management
  void setIpAddress(uint32_t ipAddress);
  Multitorque::MacAddressReturnCode setMacAddress(MacAddress &macArray);
  void getMacAddress(MacAddress &macArray);
  uint32_t getIpAddress();
  void setNetmask(uint32_t ipAddress);
  void setGateway(uint32_t gateway);
  uint32_t getNetmask();
  uint32_t getGateway();

  // Eeprom parameters management
  static constexpr const size_t NOF_INVENTORY_DATA   = 6;
  using InventoryData = std::array< uint32_t, NOF_INVENTORY_DATA > ;
  uint32_t readInventory(InventoryData& inventoryData);
  uint32_t writeInventory(uint32_t designCode,
                          uint32_t boardVer, uint32_t boardRev,
                          uint32_t serialNumber, uint32_t dateTime);



private:
  mutable std::mutex sensor_mutex;

  void analyzePacketSensor(const Ethservice::CharBuff* packetToReceive);

  Ethservice::CharBuff cb;
  Ethservice::EthInterfaceManager* bdm;
  const board_id_t bdid;


  float Fx = 0;
  float Fy = 0;
  float Fz = 0;
  float Tx = 0;
  float Ty = 0;
  float Tz = 0;
  FTSensorData raw;
};
}

#endif /* FTSENSORNEW_H_ */
