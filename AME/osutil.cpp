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

#include <cstdlib>
#include <cstring>

#include "rehab.h"
#include "osutil.h"
#include <sstream>

#include <dirent.h>
#include <sys/stat.h>
#include <thread>

namespace rehab {

/**
 * @brief Wait function in milliseconds
 * @param msec time to wait in milliseconds
 */
void spinsleep(std::chrono::milliseconds msec)
{
  std::chrono::nanoseconds timeElapsed(0);
  auto startTime=
      std::chrono::high_resolution_clock::now();
  while(timeElapsed<msec)
  {
    std::chrono::high_resolution_clock::time_point endTime=
        std::chrono::high_resolution_clock::now();

    timeElapsed = endTime - startTime;

    std::this_thread::yield(); // Allow other threads to run
  }
}

/**
 * @brief Wait function that tramsfor an integer input in milliseconds
 * @param X integer parameters which represent the time of wait
 */
void spinsleep(uint32_t X)
{
  std::chrono::milliseconds msec(X);
  spinsleep(msec);
}

/**
 * @brief generateTempName
 * @param template_
 * @return
 */
std::string generateTempName(std::string template_)
{
  std::string tempName = template_+"XXXXXX";
  char cstrTempName[255];
  std::memcpy(cstrTempName,tempName.c_str(),tempName.length());
  cstrTempName[tempName.length()]=0;
  auto fd = mkstemp(cstrTempName);
  close(fd);
  return std::string{cstrTempName};
}

/**
 * @brief generateName
 * @param baseName
 * @return
 */
std::string generateName(std::string baseName)
{
  auto timeStamp = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch() ).count();
  std::stringstream ss;
  ss << baseName << timeStamp << ".log";
  return ss.str();
}
}
