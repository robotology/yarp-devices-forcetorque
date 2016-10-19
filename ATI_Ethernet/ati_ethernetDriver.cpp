/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francisco Andrade
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "ati_ethernetDriver.h"

#include <cassert>

#include <yarp/os/LockGuard.h>

yarp::dev::ati_ethernetDriver::ati_ethernetDriver(): m_sensorReadings(6),
                                                                 m_status(yarp::dev::IAnalogSensor::AS_OK)
{
    // We fill the sensor readings only once in the constructor in this example
    // In reality, the buffer will be updated once a new measurement is avaible
   m_sensorReadings[0] = 0;
    m_sensorReadings[1] = 0;
    m_sensorReadings[2] = 0;
    
    // Set torque on x,y,z axis 
    m_sensorReadings[3] = 0;
    m_sensorReadings[4] = 0;
    m_sensorReadings[5] = 0;
    
    
    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    
}

yarp::dev::ati_ethernetDriver::~ati_ethernetDriver()
{
}

bool yarp::dev::ati_ethernetDriver::open(yarp::os::Searchable &config)
{
    yarp::os::LockGuard guard(m_mutex);
    OPort* portlist=ports.listPorts(true);
    if (ports.getLastSize()>0)
       {
           daq.open(portlist[0]);
//           ShowInformation(portlist[0])
    }
    // config should be parsed for the options of the device 
    return true;
}

bool yarp::dev::ati_ethernetDriver::close()
{
    yarp::os::LockGuard guard(m_mutex);
    daq.close();
    return true;
}

yarp::dev::ati_ethernetDriver::ati_ethernetDriver(const yarp::dev::ati_ethernetDriver& /*other*/)
{
    // Copy is disabled 
    assert(false);
}

int yarp::dev::ati_ethernetDriver::read(yarp::sig::Vector &out)
{
    yarp::os::LockGuard guard(m_mutex);

           int size=daq.read6D(pack6D,false);

    // Set force on x,y,z axis
    m_sensorReadings[0] = pack6D.Fx;
    m_sensorReadings[1] = pack6D.Fy;
    m_sensorReadings[2] = pack6D.Fz;

    // Set torque on x,y,z axis
    m_sensorReadings[3] = pack6D.Tx;
    m_sensorReadings[4] = pack6D.Ty;
    m_sensorReadings[5] = pack6D.Tz;

    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    out = m_sensorReadings;
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::getState(int /*ch*/)
{
    yarp::os::LockGuard guard(m_mutex);
        
    return m_status;
}

int yarp::dev::ati_ethernetDriver::getChannels()
{
    return 6;
}

int yarp::dev::ati_ethernetDriver::calibrateSensor()
{
    yarp::os::LockGuard guard(m_mutex);
    //read sensorSheet file? or zeroing?
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateSensor(const yarp::sig::Vector& /*value*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateChannel(int /*ch*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateChannel(int /*ch*/, double /*v*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

yarp::os::Stamp yarp::dev::ati_ethernetDriver::getLastInputStamp()
{
    return m_timestamp;
}

//void yarp::dev::ati_ethernetDriver::ShowInformation(OPort & p_Port)
//{
//    std::string deviceName = std::string(p_Port.deviceName);
//    std::string name = std::string(p_Port.name);
//    std::string serialNumber = std::string (p_Port.serialNumber);
//    int version = daq.getVersion();
//    std::cout<<"Device Name: "<<deviceName<<std::endl;
//    std::cout<<"Name: "<<name<<std::endl;
//    std::cout<<"Serial Number: "<<serialNumber<<std::endl;
//    std::cout<<"Version: "<<version<<std::endl;
//}


