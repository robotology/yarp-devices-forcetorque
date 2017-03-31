/**
 * @file
 * @author  Giuseppe Iellamo <giuseppe.iellamo@iit.it>
 * @version 1.0
 *
 * @copyright (c) IIT Fondazione Istituto Italiano
 *            di Tecnologia. All rights reserved
 *
 * @brief Small utility abstracting OS
 */

#ifndef OSUTIL
#define OSUTIL

#include <chrono>
#include <thread>
#include <vector>
#include <string>

namespace rehab
{
///
/// Sleep for X Milliseconds
/// Uses OS sleep NOT Accurate!
#define MILLISLEEP(X) do { \
    std::chrono::milliseconds tim2((X));\
    std::this_thread::sleep_for(tim2);\
} while(0)


// Time types
using time_point = std::chrono::high_resolution_clock::time_point;
using sec = std::chrono::duration<double>;

/**
 * @brief Used to trasform a frequency into nanosecond for a struc timeval
 * @param hz Frequency desired
 * @return 1000000000/hz
 */
constexpr int32_t herz2nanosec( uint32_t hz) { return 1000000000/hz; }

void spinsleep(std::chrono::milliseconds msec);

void spinsleep(uint32_t X);

std::string generateTempName(std::string template_);
std::string generateName(std::string baseName);




}
#endif // OSUTIL

