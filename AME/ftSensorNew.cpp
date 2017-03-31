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
#include "ftSensorNew.h"
#include "ethService.h"
#include "Multitorque.h"
#include "MultitorqueEncoder.h"

namespace rehab
{

using namespace Multitorque;
using namespace Ethservice;
using rehab::RC;


/**
   * @brief This function is called by the receiving thread to extract meaningful data
   * @param packetToReceive Buffer received from sensor
   */
void FtSensorNew::analyzePacketSensor(const CharBuff* packetToReceive)
{
  using namespace Multitorque;
  static BCastSensorData bCastSensorData;
  decode(bCastSensorData, (packetToReceive->content),  sizeof(CharBuff));

  std::lock_guard< std::mutex > lock(sensor_mutex);
  {
    Fx = bCastSensorData.getFx();
    Fy = bCastSensorData.getFy();
    Fz = bCastSensorData.getFz();
    Tx = bCastSensorData.getTx();
    Ty = bCastSensorData.getTy();
    Tz = bCastSensorData.getTz();
    raw[0] = bCastSensorData.getRaw0();
    raw[1] = bCastSensorData.getRaw1();
    raw[2] = bCastSensorData.getRaw2();
    raw[3] = bCastSensorData.getRaw3();
    raw[4] = bCastSensorData.getRaw4();
    raw[5] = bCastSensorData.getRaw5();
  }
}

/**
 * @brief FtSensorNew::isConnectivityOk
 * @return
 */
bool FtSensorNew::isConnectivityOk()
{
  return true;
}

/**
 * @brief Create the interface to the control sensor device
 * @param bdm_ Pointer of Ethservice::EthInterfaceManager class
 * @param bdid_ Integer value which represents board identity
 */
FtSensorNew::FtSensorNew(Ethservice::EthInterfaceManager* bdm_, board_id_t bdid_):
  bdm{bdm_}, bdid{bdid_}
{
  memset( &this->cb, 0x0, sizeof(Ethservice::CharBuff));
  bdm->installUdpDatagramHandler(bdid, [this](const CharBuff* bf) {this->analyzePacketSensor(bf); });
}

/**
   * @brief The destructor is not virtual because the class is Final
   *        and is meant not to be overriden
   */
FtSensorNew::~FtSensorNew()
{  }

/**
   * @brief Assign values Force and Torque to array Sensor Data
   * @param ftData Assign to FTSensorData array all value of force in N
   *        and torque in Nm
   * @return 0 means no errors
   */
int32_t FtSensorNew::getFTData (FTSensorData &ftData)
{
  std::lock_guard < std::mutex > lock(sensor_mutex);
  ftData[0] = Fx / 1000.0 ;
  ftData[1] = Fy / 1000.0 ;
  ftData[2] = Fz / 1000.0 ;
  ftData[3] = Tx / 1000.0 ;
  ftData[4] = Ty / 1000.0 ;
  ftData[5] = Tz / 1000.0 ;
  return 0;
}

/**
   * @brief Gets all 6 values for specified row of matrix
   * @param rawFTData FTSensorData array which contains all value of force in N
   * @return 0 means no errors
   */
int32_t FtSensorNew::getRawFTData (FTSensorData &rawFTData)
{
  std::lock_guard < std::mutex > lock(sensor_mutex);
  memcpy(rawFTData.data(), raw.data(), sizeof(raw));
  return 0;
}

/**
   * @brief Sets all calibration offsets from sensor device
   */
int32_t FtSensorNew::calibrateOffsets()
{
  CalibrateOffsets db;
  cb.size = encode(db, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);
  if (res < 0 )
  {
    std::cerr << "Send calibrateOffsets Error <<"<< res<<std::endl;
    return -1;
  }

  // Check that after calibration we're reading no offsets
  // for real. Cause sometimes if noise is very high calibration
  // may not converge.
  MILLISLEEP(310);

  constexpr float maxForceAfterCalibration{50000.0};
  constexpr float maxForceAfterCalibrationImp{30000.0};
  constexpr float maxTorqueAfterCalibration{4500.0};
  constexpr float maxTorqueAfterCalibrationImp{3500.0};

  std::lock_guard < std::mutex > lock(sensor_mutex);
  if ( std::abs(this->Fx)  > maxForceAfterCalibration)
    return -1;
  if ( std::abs(this->Fy) > maxForceAfterCalibration)
    return -1;
  if ( std::abs(this->Fz) > maxForceAfterCalibrationImp)
    return -1;

  if ( std::abs(this->Tx) > maxTorqueAfterCalibrationImp)
    return -1;
  if ( std::abs(this->Ty) > maxTorqueAfterCalibrationImp)
    return -1;
  if ( std::abs(this->Tz) > maxTorqueAfterCalibration)
    return -1;

  return 0;
}

/**
   * @brief Gets all calibration offsets from sensor device
   * @param calibrationOffsets FTCalibrationOffsets array which
   *        contains all value of calibration offsets
   */
void FtSensorNew::getCalibrationOffsets(FTCalibrationOffsets &calibrationOffsets)
{
  GetCalibrationOffsets coff;
  cb.size = encode(coff, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << "Send GetCalibrationOffsets Error <<"<< res<<std::endl;
  }

  ReplyGetCalibrationOffsets rb;
  res = bdm->recvTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << "Recv ReplyGetCalibrationOffsets Error <<"<< res<<std::endl;
  }

  decode(rb, cb.content, cb.size);

  auto result = checkMessageConsistency(rb.getHeaderId(), MESSAGE_ID__REPLY_CALIBRATE_OFFSETS);

  calibrationOffsets[0] = rb.getC0();
  calibrationOffsets[1] = rb.getC1();
  calibrationOffsets[2] = rb.getC2();
  calibrationOffsets[3] = rb.getC3();
  calibrationOffsets[4] = rb.getC4();
  calibrationOffsets[5] = rb.getC5();

}


/**
   * @brief FtSensorNew::setSampleStreamPol
   * @param pol SampleStreamPolicy value that can
   *        be 0 = OFF or 1 = normal
   * @param rate rate in unit of half milliseconds
   */
void FtSensorNew::setSampleStreamPol(SampleStreamPolicy pol,
                                     uint32_t rate)
{
  SetSampleStreamPolicy set;
  set.setPolicy(pol);
  set.setRate(rate);
  cb.size = encode(set, cb.content,  sizeof(CharBuff));
  bdm->sendTcpFromBoard(bdid, &cb);
}

/**
   * @brief This function returns release version of the board
   * @return Board release info
   */
uint32_t FtSensorNew::getBoardInfo()
{
  GetBoardInfo gbi;
  cb.size = encode(gbi, cb.content, sizeof(Ethservice::CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << "Send GetBoardInfo Error <<"<< res<<std::endl;
  }

  ReplyBoardInfo rpl;
  res = bdm->recvTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << "Recv ReplyBoardInfo Error <<"<< res<<std::endl;
  }

  decode(rpl, cb.content, cb.size);

  auto result = checkMessageConsistency(rpl.getHeaderId(), MESSAGE_ID__REPLY_BOARD_INFO);

  return rpl.getRelease();

}

/**
   * @brief This function set data in to Matrix Calibration
   * @param row Row of matrix calibration
   * @param data FTSensorData array which contains all value of force in N
   *        and torque in Nm
   */
void FtSensorNew::setMtxRow(uint32_t row, FTSensorData &data)
{
  SetCalibrationMatrixRow set;

  set.setRow(row);
  set.setC0(data[0]);
  set.setC1(data[1]);
  set.setC2(data[2]);
  set.setC3(data[3]);
  set.setC4(data[4]);
  set.setC5(data[5]);
  cb.size = encode(set, cb.content,  sizeof(CharBuff));
  bdm->sendTcpFromBoard(bdid, &cb);

}

/**
    * @brief This function get data from Sensor and put them in to Matrix
    * @param row Row of matrix calibration
    * @param mtxRow FTSensorData array which contains all value of force in N
    *        and torque in Nm
    */
void FtSensorNew::getMtxRow(uint32_t row, FTSensorData &mtxRow)
{
  GetCalibrationMatrixRow coff;
  coff.setRow(row);
  cb.size = encode(coff, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " Send GetCalibrationMatrixRow Error"<<std::endl;
  }

  ReplyGetCalibrationMatrixRow rb;
  res = bdm->recvTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " recv ReplyGetCalibrationMatrixRow Error"<<std::endl;
  }

  decode(rb, cb.content, cb.size);

  auto result = checkMessageConsistency(rb.getHeaderId(), MESSAGE_ID__REPLY_GET_CALIBRATION_MATRIX_ROW);

  mtxRow[0] = rb.getC0();
  mtxRow[1] = rb.getC1();
  mtxRow[2] = rb.getC2();
  mtxRow[3] = rb.getC3();
  mtxRow[4] = rb.getC4();
  mtxRow[5] = rb.getC5();
}

/**
    * @brief FtSensorNew::saveParamsOnFlash
    */
void FtSensorNew::saveParamsOnFlash()
{
  SaveParamsOnFlash sv;
  cb.size = encode(sv, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " Send saveParamOnFlash Error"<<std::endl;
  }

}

/**
 * @brief Sets IP address of board
 * @param ipAddress value of address IP of sensor
 */
void FtSensorNew::setIpAddress(uint32_t ipAddress)
{
  SetIpAddress set;
  set.setIpAddress(ntohl(ipAddress));
  cb.size = encode(set, cb.content,  sizeof(CharBuff));
  bdm->sendTcpFromBoard(bdid, &cb);

}

/**
 * @brief Gets IP address of board
 * @return Value of address IP of sensor
 */
uint32_t FtSensorNew::getIpAddress()
{
  GetIpAddress ipAddr;
  cb.size = encode(ipAddr, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " Send getIpAddress Error"<<std::endl;
  }

  ReplyGetIpAddress ra;
  res = bdm->recvTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " recv ReplyGetIpAddress Error"<<std::endl;
  }

  decode(ra, cb.content, cb.size);

  auto result = checkMessageConsistency(ra.getHeaderId(), MESSAGE_ID__REPLY_GET_IP_ADDRESS);

  return ra.getIpAddress();


}


/**
 * @brief Sets MAC address of board
 * @param macArray Array wich contains Mac address ( e.g macArray[] = 00 ,
 *        macArray[1] = 11 etc)
 * @return MacAddressReturnCode value , this means 0 if no errors
 *         otherwise 1
 */
Multitorque::MacAddressReturnCode FtSensorNew::setMacAddress(MacAddress &macArray)
{
  SetMacAddress set;
  set.setM0(macArray[0]);
  set.setM1(macArray[1]);
  set.setM2(macArray[2]);
  set.setM3(macArray[3]);
  set.setM4(macArray[4]);
  set.setM5(macArray[5]);
  set.setM6(0);
  set.setM7(0);

  cb.size = encode(set, cb.content,  sizeof(CharBuff));
  bdm->sendTcpFromBoard(bdid, &cb);

  ReplySetMacAddress rsmac;
  bdm->recvTcpFromBoard(bdid, &cb);
  decode(rsmac, cb.content, cb.size);

  auto result = checkMessageConsistency(rsmac.getHeaderId(), MESSAGE_ID__REPLY_SET_MAC_ADDRESS);

  return (rsmac.getMacrc());
}

/**
 * @brief Gets MAC address of board
 * @param macArray Array wich contains Mac address ( e.g macArray[] = 00 ,
 *        macArray[1] = 11 etc)
 */
void FtSensorNew::getMacAddress(MacAddress &macArray)
{

  GetMacAddress mac;
  cb.size = encode(mac, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " Send getMacAddress Error"<<std::endl;
  }

  ReplyGetMacAddress ra;
  res = bdm->recvTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " recv ReplyGetIpAddress Error"<<std::endl;
  }

  decode(ra, cb.content, cb.size);

  auto result = checkMessageConsistency(ra.getHeaderId(), MESSAGE_ID__REPLY_GET_MAC_ADDRESS);

  macArray[0] = ra.getM0();
  macArray[1] = ra.getM1();
  macArray[2] = ra.getM2();
  macArray[3] = ra.getM3();
  macArray[4] = ra.getM4();
  macArray[5] = ra.getM5();
  macArray[6] = ra.getM6();
  macArray[7] = ra.getM7();
}

/**
 * @brief Sets Net Mask address of board
 * @param netMask value of net mask
 */
void FtSensorNew::setNetmask(uint32_t netmask)
{
  SetNetmask sn;
  sn.setNetmask(ntohl(netmask));
  cb.size = encode(sn, cb.content,  sizeof(CharBuff));
  bdm->sendTcpFromBoard(bdid, &cb);
}

/**
 * @brief Gets Net Mask address of board
 * @return Value of net mask
 */
uint32_t FtSensorNew::getNetmask()
{
  GetNetmask netmask;
  cb.size = encode(netmask, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " Send getNetMask Error"<<std::endl;
  }

  ReplyGetNetmask rgn;
  res = bdm->recvTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " recv getNetMask Error"<<std::endl;
  }

  decode(rgn, cb.content, cb.size);

  auto result = checkMessageConsistency(rgn.getHeaderId(), MESSAGE_ID__REPLY_GET_NETMASK);

  //i need to use htonl to move host long to network
  return htonl(rgn.getNetmask());
}


/**
 * @brief Sets Gateway address of board
 * @param gateWay gateway address of board
 */
void FtSensorNew::setGateway(uint32_t gateway)
{
  SetGateway sg;
  sg.setGateway(ntohl(gateway));
  cb.size = encode(sg, cb.content,  sizeof(CharBuff));
  bdm->sendTcpFromBoard(bdid, &cb);

}

/**
 * @brief Sets Gateway address of board
 * @return Gateway address of board
 */
uint32_t FtSensorNew::getGateway()
{
  GetGateway gateway;
  cb.size = encode(gateway, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " Send getGateWay Error"<<std::endl;
  }

  ReplyGetGateway rgw;
  res = bdm->recvTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " recv getGateWay Error"<<std::endl;
  }

  decode(rgw, cb.content, cb.size);

  auto result = checkMessageConsistency(rgw.getHeaderId(), MESSAGE_ID__REPLY_GET_GATEWAY);

  //i need to use htonl to move host long to network
  return htonl(rgw.getGateway());
}

/**
 * @brief Function taht returns all data of inventory
 *        from flash memory
 * @param inventoryData Array of int32 which contains
 *        all inventory data
 * @return Data Inventory read from flash
 */
uint32_t FtSensorNew::readInventory(InventoryData& inventoryData)
{
  LoadInventoryDataFromFlash inv;
  cb.size = encode(inv, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " Send LoadInventoryDataOnFlash Error"<<std::endl;
  }

  ReplyLoadInventoryDataFromFlash rinv;
  res = bdm->recvTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " recv ReplyReadInventory Error"<<std::endl;
  }

  decode(rinv, cb.content, cb.size);

  auto result = checkMessageConsistency(rinv.getHeaderId(), MESSAGE_ID__REPLY_LOAD_INVENTORY_DATA_FROM_FLASH);

  inventoryData[0] = rinv.getDesignCode();
  inventoryData[1] = rinv.getBoardVer();
  inventoryData[2] = rinv.getBoardRev();
  inventoryData[3] = rinv.getSerialNumber();
  inventoryData[4] = rinv.getDateTime();
  return 0;
}

/**
 * @brief FtSensorNew::writeInventory
 * @param designCode Design code of board
 * @param boardVer Board version
 * @param boardRev Board Revision
 * @param serialNumber Serial Number of board
 * @param dateTime Date of flashing eeprom
 */
uint32_t FtSensorNew::writeInventory(uint32_t designCode, uint32_t boardVer,
                                 uint32_t boardRev , uint32_t serialNumber,
                                 uint32_t dateTime)
{

  SaveInventoryDataOnFlash inv;
  inv.setDesignCode(designCode);
  inv.setBoardVer(boardVer);
  inv.setBoardRev(boardRev);
  inv.setSerialNumber(serialNumber);
  inv.setDateTime(dateTime);
  cb.size = encode(inv, cb.content,  sizeof(CharBuff));
  auto res = bdm->sendTcpFromBoard(bdid, &cb);

  if (res == -1)
  {
    std::cerr << " Send SaveInventoryDataOnFlash Error"<<std::endl;
    return -1;
  }
  return 0;
}
}


