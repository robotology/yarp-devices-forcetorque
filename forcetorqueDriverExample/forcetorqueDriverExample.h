/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#ifndef YARP_FORCETORQUEDRIVEREXAMPLE_H
#define YARP_FORCETORQUEDRIVEREXAMPLE_H

#include <yarp/os/Mutex.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/PreciselyTimed.h>

#include <yarp/sig/Vector.h>

namespace yarp {
namespace dev {

class forcetorqueDriverExample : public yarp::dev::IAnalogSensor,
                                 public yarp::dev::DeviceDriver,
                                 public yarp::dev::IPreciselyTimed
{
private:
    // Prevent copy 
    forcetorqueDriverExample(const forcetorqueDriverExample & other);
    forcetorqueDriverExample & operator=(const forcetorqueDriverExample & other);
    
    // Use a mutex to avoid race conditions
    yarp::os::Mutex m_mutex;
    
    // Buffers of sensor data and timestamp
    yarp::sig::Vector m_sensorReadings;
    yarp::os::Stamp m_timestamp;
    
    // Status of the sensor 
    int m_status;
    
public:
    forcetorqueDriverExample();
    virtual ~forcetorqueDriverExample();

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

