/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francesco Romano
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#ifndef YARP_AMTIPLATFORMSDRIVER_H
#define YARP_AMTIPLATFORMSDRIVER_H

#include <yarp/os/Mutex.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/PreciselyTimed.h>
#include "IMultipleForcePlates.h"

#include <yarp/sig/Vector.h>

namespace yarp {
	namespace dev {
		class AMTIPlatformsDriver;
	}
}

class yarp::dev::AMTIPlatformsDriver : public yarp::dev::IMultipleForcePlates,
										public yarp::dev::DeviceDriver,
										public yarp::dev::IPreciselyTimed                             
{
private:
    // Prevent copy 
    AMTIPlatformsDriver(const AMTIPlatformsDriver &other);
    AMTIPlatformsDriver& operator=(const AMTIPlatformsDriver &other);
    
    // Use a mutex to avoid race conditions
    yarp::os::Mutex m_mutex;
    
    // Buffers of sensor data and timestamp
    yarp::sig::Vector m_sensorReadings;
    yarp::os::Stamp m_timestamp;
    
    // Properties of the sensor 
	unsigned m_numOfPlatforms;
	unsigned m_channelSize;
    
    //private class for reading from the sensor
    class AMTIReaderThread;
    AMTIReaderThread *m_reader;

    int m_status;

public:
    AMTIPlatformsDriver();
    virtual ~AMTIPlatformsDriver();

    // DeviceDriver interface 
    bool open(yarp::os::Searchable &config);
    bool close();

    //IMultipleForcePlates interface
    virtual int getNumberOfPlatforms();
    virtual int getPlatformIndexForPlatformID(const std::string& platformID);

    virtual int getLastMeasurementForPlateAtIndex(const unsigned platformIndex,
        yarp::sig::Vector& measurement,
        yarp::os::Stamp *timestamp);
    
    // IPreciselyTimed interface
    virtual yarp::os::Stamp getLastInputStamp();

};

#endif //end of YARP_AMTIPLATFORMSDRIVER_H
