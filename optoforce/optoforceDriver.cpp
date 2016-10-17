/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "forcetorqueDriverExample.h"

#include <cassert>

#include <yarp/os/LockGuard.h>

yarp::dev::forcetorqueDriverExample::forcetorqueDriverExample(): m_sensorReadings(6),
                                                                 m_status(yarp::dev::IAnalogSensor::AS_OK)
{
    // We fill the sensor readings only once in the constructor in this example
    // In reality, the buffer will be updated once a new measurement is avaible
    
    // Set force on x,y,z axis 
    m_sensorReadings[0] = 1.0;
    m_sensorReadings[1] = 2.0;
    m_sensorReadings[2] = 3.0;
    
    // Set torque on x,y,z axis 
    m_sensorReadings[3] = 0.1;
    m_sensorReadings[4] = 0.2;
    m_sensorReadings[5] = 0.3;
    
    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    
}

yarp::dev::forcetorqueDriverExample::~forcetorqueDriverExample()
{
}

bool yarp::dev::forcetorqueDriverExample::open(yarp::os::Searchable &config)
{
    yarp::os::LockGuard guard(m_mutex);
    
    // config should be parsed for the options of the device 
    return true;
}

bool yarp::dev::forcetorqueDriverExample::close()
{
    yarp::os::LockGuard guard(m_mutex);
    
    return true;
}

yarp::dev::forcetorqueDriverExample::forcetorqueDriverExample(const yarp::dev::forcetorqueDriverExample& /*other*/)
{
    // Copy is disabled 
    assert(false);
}

int yarp::dev::forcetorqueDriverExample::read(yarp::sig::Vector &out)
{
    yarp::os::LockGuard guard(m_mutex);
    
    out = m_sensorReadings;
    
    return m_status;
}

int yarp::dev::forcetorqueDriverExample::getState(int /*ch*/)
{
    yarp::os::LockGuard guard(m_mutex);
        
    return m_status;
}

int yarp::dev::forcetorqueDriverExample::getChannels()
{
    return 6;
}

int yarp::dev::forcetorqueDriverExample::calibrateSensor()
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::forcetorqueDriverExample::calibrateSensor(const yarp::sig::Vector& /*value*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::forcetorqueDriverExample::calibrateChannel(int /*ch*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::forcetorqueDriverExample::calibrateChannel(int /*ch*/, double /*v*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

yarp::os::Stamp yarp::dev::forcetorqueDriverExample::getLastInputStamp()
{
    return m_timestamp;
}


