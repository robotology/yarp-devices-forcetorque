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
	//char * AXES[] = { "Fx", "Fy", "Fz", "Tx", "Ty", "Tz" };	/* The names of the force and torque axes. */

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

