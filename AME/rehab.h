/**
 * @file
 * @author  Giuseppe Iellamo <giuseppe.iellamo@iit.it>
 * @author  Mirco Di Salvo <mirco.disalvo@iit.it>
 * @version 1.0
 *
 * @copyright (c) IIT Fondazione Istituto Italiano
 *            di Tecnologia. All rights reserved
 *
 * @brief Small utilities
 */

#ifndef REHAB_H_
#define REHAB_H_

// Language dependencies
#include <cstdint>
#include <string>
#include <random>
#include <vector>
#include <functional>

#include <iostream>

#define _POSIX_C_SOURCE 200809L

#define REHAB_MAJOR_VERSION 1
#define REHAB_MINOR_VERSION 0

/**
 *\brief Namespace that contains all features about Arbot project
 */
namespace rehab
{

using arr2D = std::array<double, 2>;
using arr2I = std::array<int32_t, 2>;

// Distance between two vectors

// Definition of pi, in boost is more accurate but is needed?
constexpr double pi { 3.14159265f };
constexpr double pi_180 { pi / 180.f };
constexpr double pi_180_inv { 180.f / pi };

//Maximun of packet size
constexpr size_t PACKET_MAX_SIZE{256};
/**
 * \brief Struct used as a descriptor for the packet going
 * up and down from the boards
 */
template <size_t N=PACKET_MAX_SIZE>
struct CharBuff_t
{
  CharBuff_t() : content{0}, size{} {}
  uint8_t content[N]; /// all data static?
  size_t size;
  bool isFull() const { return size>=N; }
  void reset() { size=0; }
};

using CharBuff=CharBuff_t<PACKET_MAX_SIZE>;

// Non member Begin End to allow range based for loops
/**
 * @brief Function that returns first character of buffer
 * @param c Input buffer
 * @return First byte in CharBuffer
 */
template < size_t size>
uint8_t* begin(CharBuff_t<size> &c)
{

    return &c.content[0];
}

/**
 * @brief Function that returns last character of buffer
 * @param c Input buffer
 * @return End of Buff iterator
 */
template < size_t size>
uint8_t* end(CharBuff_t<size>& c)
{
    return &c.content[0] + c.size;
}
/************************************************************************/

/*!
 * \brief Sign for T. Is not the mathematical sign because if T is 0 returns 1...
 */
template<typename T>
constexpr int sign(T t)
{
  return (t < 0) ? -1 : 1;
}


/**
 * @brief Trasforms angle value in radians from degree
 * @param angleInDegree Input angle value in degree
 * @return Angle in radians
 */
constexpr double deg2rad(double angleInDegree)
{
  return angleInDegree * pi_180;
}


/**
 * @brief Trasforms angle value in degree from radians
 * @param angleRad Input angle value in radians
 * @return Angle in degree
 */
constexpr double rad2deg(double angleRad)
{
  return angleRad * pi_180_inv;
}

///enum for erros on trasmission protocol
enum class RC
{
      OK = 0x0,
      NOCONN,
      HWFAULT,
      WRONG_PARAM,
      ENC_ERROR = 10,
      FT_ERROR = 11,
      WRONG_BASE_MSWITCH,
      UNKNOWN_ERR
};

/**
 * @brief Wraps for checking error, needed because some libraries define OK!
 * @param rc enum of errors
 * @return true if RC is OK needed
 */
inline constexpr bool isRCOk( RC rc )
{
  return (rc == RC::OK);
}

}
#endif /* REHAB_H_ */
