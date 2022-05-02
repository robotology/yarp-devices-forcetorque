/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francisco Andrade
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "optoforceDriver.h"

#include <cassert>

#include <yarp/os/LockGuard.h>

#include <string>
#include <sstream>

yarp::dev::optoforceDriver::optoforceDriver(): m_sensorReadings(6),
                                                                 m_status(yarp::dev::IAnalogSensor::AS_OK)
                                                                    ,calibFactor(6)
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
    
    calibFactor[0] = 0;
     calibFactor[1] = 0;
     calibFactor[2] = 0;

     // Set torque on x,y,z axis
     calibFactor[3] = 0;
     calibFactor[4] = 0;
     calibFactor[5] = 0;

    
    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    
}

yarp::dev::optoforceDriver::~optoforceDriver()
{

}

bool yarp::dev::optoforceDriver::open(yarp::os::Searchable &config)
{
    yarp::os::LockGuard guard(m_mutex);
    OPort* portlist=ports.listPorts(true);
    if (ports.getLastSize()>0)
       {
           daq.open(portlist[0]);
//           ShowInformation(portlist[0])
    }

    prop.fromConfigFile("calib.ini");

   //format
     yInfo(prop.findGroup("Axis").toString());
    //"Calib Fx:
    yInfo(prop.findGroup("Fx").toString());
  //"Calib Fy:
    yInfo(prop.findGroup("Fy").toString());
  //Calib Fz
    yInfo(prop.findGroup("Fz").toString());
  //Calib Tx
    yInfo(prop.findGroup("Tx").toString());
  //Calib Ty
    yInfo(prop.findGroup("Ty").toString());
  //Calib Tz
    yInfo(prop.findGroup("Tz").toString());

    //store calibration

        //prop.findGroup("Fx").tail().get(0).asFloat64()/prop.findGroup("Fx").tail().get(0).asFloat64()
//    double  test;
//       test=prop.findGroup("Fx").tail().get(0).asFloat64()/prop.findGroup("Fx").tail().get(1).asFloat64();
//        std::ostringstream os;
//        os << test;
//        std::string str = os.str();
//        yInfo(str);
         calibFactor[0] = prop.findGroup("Fx").tail().get(0).asFloat64()/prop.findGroup("Fx").tail().get(1).asFloat64();
      calibFactor[1] = prop.findGroup("Fy").tail().get(0).asFloat64()/prop.findGroup("Fy").tail().get(1).asFloat64();
     calibFactor[2] = prop.findGroup("Fz").tail().get(0).asFloat64()/prop.findGroup("Fz").tail().get(1).asFloat64();

     // Set torque on x,y,z axis
     calibFactor[3] = prop.findGroup("Tx").tail().get(0).asFloat64()/prop.findGroup("Tx").tail().get(1).asFloat64();
     calibFactor[4] = prop.findGroup("Ty").tail().get(0).asFloat64()/prop.findGroup("Ty").tail().get(1).asFloat64();
     calibFactor[5] = prop.findGroup("Tz").tail().get(0).asFloat64()/prop.findGroup("Tz").tail().get(1).asFloat64();
  // config should be parsed for the options of the device
  return true;
}

bool yarp::dev::optoforceDriver::close()
{
    yarp::os::LockGuard guard(m_mutex);
    daq.close();
    return true;
}

yarp::dev::optoforceDriver::optoforceDriver(const yarp::dev::optoforceDriver& /*other*/)
{
    // Copy is disabled 
    assert(false);
}

int yarp::dev::optoforceDriver::read(yarp::sig::Vector &out)
{
    yarp::os::LockGuard guard(m_mutex);

           int size=daq.read6D(pack6D,false);

    // Set force on x,y,z axis
    m_sensorReadings[0] = pack6D.Fx*calibFactor[0];
    m_sensorReadings[1] = pack6D.Fy*calibFactor[1];
    m_sensorReadings[2] = pack6D.Fz*calibFactor[2];

    // Set torque on x,y,z axis
    m_sensorReadings[3] = pack6D.Tx*calibFactor[3];
    m_sensorReadings[4] = pack6D.Ty*calibFactor[4];
    m_sensorReadings[5] = pack6D.Tz*calibFactor[5];

    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    out = m_sensorReadings;
    
    return m_status;
}

int yarp::dev::optoforceDriver::getState(int /*ch*/)
{
    yarp::os::LockGuard guard(m_mutex);
        
    return m_status;
}

int yarp::dev::optoforceDriver::getChannels()
{
    return 6;
}

int yarp::dev::optoforceDriver::calibrateSensor()
{
    yarp::os::LockGuard guard(m_mutex);
    //read sensorSheet file? or zeroing?
    return m_status;
}

int yarp::dev::optoforceDriver::calibrateSensor(const yarp::sig::Vector& /*value*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::optoforceDriver::calibrateChannel(int /*ch*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::optoforceDriver::calibrateChannel(int /*ch*/, double /*v*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

yarp::os::Stamp yarp::dev::optoforceDriver::getLastInputStamp()
{
    return m_timestamp;
}

//void yarp::dev::optoforceDriver::ShowInformation(OPort & p_Port)
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


