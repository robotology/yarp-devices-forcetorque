/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francisco Andrade
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#ifndef YARP_ati_ethernetDriver_H
#define YARP_ati_ethernetDriver_H

#include <yarp/os/Mutex.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/PreciselyTimed.h>

#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>

#include <iostream>

#ifdef _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib")
#else
	#include <arpa/inet.h>
    #include <sys/socket.h>
	#include <netdb.h>
#endif

#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

 #include <vector>

#include <tinyxml.h>

#define PORT 49152 /* Port the Net F/T always uses */
#define COMMAND 2 /* Command code 2 starts streaming */

typedef unsigned char byte;
/* Typedefs used so integer sizes are more explicit */
typedef struct response_struct {
    uint32_t rdt_sequence;
    uint32_t ft_sequence;
    uint32_t status;
    int FTData[6];
} RESPONSE;

namespace sys{namespace socket{}}
namespace yarp {
namespace dev {

class ati_ethernetDriver : public yarp::dev::IAnalogSensor,
                                 public yarp::dev::DeviceDriver,
                                 public yarp::dev::IPreciselyTimed
{
private:
    // Prevent copy 
    ati_ethernetDriver(const ati_ethernetDriver & other);
    ati_ethernetDriver & operator=(const ati_ethernetDriver & other);
    
    // Use a mutex to avoid race conditions
    yarp::os::Mutex m_mutex;
    
    // Buffers of sensor data and timestamp
    yarp::sig::Vector m_sensorReadings;
    yarp::os::Stamp m_timestamp;
    
    // Status of the sensor 
    int m_status;

    // Calibration matrix
    yarp::sig::Matrix cMatrix;
    double countsperForce;
    double countsperTorque;

    //Variables used in the original exmample
#ifdef _WIN32
	SOCKET socketHandle;		/* Handle to UDP socket used to communicate with Net F/T. */
	WSADATA wsaData;
    WORD wVersionRequested;
#else
	int socketHandle;			/* Handle to UDP socket used to communicate with Net F/T. */
#endif
	struct sockaddr_in addr;	/* Address of Net F/T. */
	struct hostent *he;			/* Host entry for Net F/T. */
	byte request[8];			/* The request data sent to the Net F/T. */
	RESPONSE resp;				/* The structured response received from the Net F/T. */
	byte response[36];			/* The raw response data received from the Net F/T. */
	int i;						/* Generic loop/array index. */
	int err;					/* Error status of operations. */

    bool inline stringToDouble(const std::string & inStr, double & outDouble)
        {
           outDouble = std::atof(inStr.c_str());
            return true;
        }


    std::string inline intToString(const int inInt)
        {
            std::stringstream ss;
            ss << inInt;
            return ss.str();
        }

    bool inline splitString(const std::string & inStr, std::vector<std::string> & pieces)
        {
            std::istringstream iss(inStr);
            pieces.resize(0);
            do
            {
                std::string sub;
                iss >> sub;
                if( sub != "" )
               {
                    pieces.push_back(sub);
                }
           } while (iss);
             return true;
        }

    bool inline vector6FromString(const std::string & vector_str, yarp::sig::Vector & out)
      {
           std::vector<std::string> pieces;
           std::vector<double> xyz;
           splitString(vector_str,pieces);
           for (unsigned int i = 0; i < pieces.size(); ++i)
           {
             if (pieces[i] != ""){
              double newDouble;
              if( stringToDouble(pieces[i],newDouble) )
              {
                 xyz.push_back(newDouble);
                 yDebug("vector6FromString : inserting double value "+ pieces[i]);
              }
              else
            {
                  std::string errStr = "Unable to parse component [" + pieces[i] + "] to a double (while parsing a vector value)";
                  yError("vector6FromString",errStr.c_str());
                  return false;
              }
             }
           }

           if (xyz.size() != 6)
           {
               std::string errStr = "Parser found " + intToString(xyz.size())  + " elements but 6 expected while parsing vector [" + vector_str + "]";
              yError("vector6FromString",errStr.c_str());
              return false;
          }

          out[0]  = xyz[0];
          out[1] = xyz[1];
          out[2] = xyz[2];
          out[3]  = xyz[3];
          out[4] = xyz[4];
          out[5] = xyz[5];

          return true;
      }

public:
    ati_ethernetDriver();
    virtual ~ati_ethernetDriver();

    // DeviceDriver interface 
    bool open(yarp::os::Searchable &config);
    bool close();

    // IAnalogSensor interface
    virtual int read(yarp::sig::Vector &out);
    virtual int getState(int ch);
    virtual int getChannels();
    virtual int calibrateChannel(int ch, double v);
    virtual int calibrateSensor();
    virtual int calibrateSensor(const yarp::sig::Vector& value);
    virtual int calibrateChannel(int ch);
    
    // IPreciselyTimed interface
    virtual yarp::os::Stamp getLastInputStamp();
};

}
}

#endif

