/*
 * Copyright (C) 2017 iCub Facility
 * Authors: Francisco Andrade
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#ifndef AMEDRIVER
#define AMEDRIVER

#include <yarp/os/Mutex.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/PreciselyTimed.h>

#include <yarp/sig/Vector.h>

#include <yarp/os/Property.h>

#include <stdio.h>
#include <iostream>
#include <cassert>
#include <string.h>
#include "rehab.h"
#include "osutil.h"
#include "ethService.h"
#include "ftSensorNew.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <ifaddrs.h>

namespace yarp {
namespace dev {

class amedriver : public yarp::dev::IAnalogSensor,
                                 public yarp::dev::DeviceDriver,
                                 public yarp::dev::IPreciselyTimed
{
private:
    // Prevent copy
    amedriver(const amedriver & other);
    amedriver & operator=(const amedriver & other);

    // Use a mutex to avoid race conditions
    yarp::os::Mutex m_mutex;

    // Buffers of sensor data and timestamp
    yarp::sig::Vector m_sensorReadings;
    yarp::os::Stamp m_timestamp;

    //To open config files in this case the calib.ini file
     yarp::os::Property prop;

    // Status of the sensor
    int m_status;

   // Pointer for the ftSensor class
    shared_ptr<rehab::FtSensorNew> ft;

    //Vector to store calibration factor
    yarp::sig::Vector calibFactor;

public:
    amedriver();
    virtual ~amedriver();

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

#endif // AMEDRIVER

