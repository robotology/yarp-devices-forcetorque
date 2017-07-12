/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francisco Andrade
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "ftshoeDriver.h"

#include <cassert>

#include <yarp/os/LockGuard.h>
#include <yarp/os/LogStream.h>

#include <yarp/math/api.h>
#include <yarp/math/Math.h>

using namespace yarp::math;

#include <string>
#include <sstream>


yarp::dev::ftshoeDriver::ftshoeDriver() : f_sensorReadings(6),
                                          s_sensorReadings(6),
                                          devout_data(6),
                                          f_sensor_p(0),
                                          s_sensor_p(0),
                                          f_status(yarp::dev::IAnalogSensor::AS_OK),
                                          s_status(yarp::dev::IAnalogSensor::AS_OK),
                                          p_status(yarp::dev::IAnalogSensor::AS_OK),
                                          fts_offset(3),
                                          s_fts_to_out_R(3,3)
{
    // Initialize input buffers with zero values
    f_sensorReadings.zero();
    s_sensorReadings.zero();

    // initialize output data buffer
    devout_data.zero();

    // initialize ftShoe settings
    fts_offset.zero();
    s_fts_to_out_R.zero();

    // When you update the sensor readings, you also need to update the timestamp
    f_timestamp.update();
    s_timestamp.update();
    // output timestamp is the mean of timestamps of each sensor
    double mean = (f_timestamp.getTime() + s_timestamp.getTime()) / 2.0;
    devout_timestamp.update(mean);
}

yarp::dev::ftshoeDriver::~ftshoeDriver()
{

}

bool yarp::dev::ftshoeDriver::open(yarp::os::Searchable &config)
{
    yarp::os::LockGuard guard(p_mutex);

    prop.fromString(config.toString().c_str());

    if (prop.check("fts_offset") && prop.find("fts_offset").isList() && prop.find("fts_offset").asList()->size() == 3)
    {
        for (int i = 0; i < 3; ++i)
        {
            fts_offset(i) = prop.find("fts_offset").asList()->get(i).asDouble();
        }
    }
    else
    {
        yError() << "ftshoeDriver : offset parameter not present or wrongly defined";
        return false;
    }

    if (!prop.check("rear_fts_to_out_R"))
    {
        yWarning() << "ftshoeDriver : rear fts to output system of reference rotation matrix not found, set to identity";
        s_fts_to_out_R = eye(3);
    }
    else
    {
        if (prop.find("rear_fts_to_out_R").isList() && prop.find("rear_fts_to_out_R").asList()->size() == 9)
        {
            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int rowMajorIndex = 3 * r + c;
                    s_fts_to_out_R(r, c) = prop.find("rear_fts_to_out_R").asList()->get(rowMajorIndex).asDouble();
                }
            }
        }
        else
        {
            yError() << "ftshoeDriver : rear fts to output system of reference rotation matrix wrongly defined";
            return false;
        }
    }
    return true;
}

bool yarp::dev::ftshoeDriver::close()
{
    return true;
}

yarp::dev::ftshoeDriver::ftshoeDriver(const yarp::dev::ftshoeDriver& /*other*/)
{
    // Copy is disabled
    assert(false);
}

int yarp::dev::ftshoeDriver::read(yarp::sig::Vector &out)
{
    out.resize(6);
    yarp::os::LockGuard guard(p_mutex);
    f_status = f_sensor_p->read(f_sensorReadings);
    f_timestamp.update();
    s_status = s_sensor_p->read(s_sensorReadings);
    s_timestamp.update();
    // move data in first fts SoR to second fts SoR
    // right now assuming fts SoRs are aligned R == eye(3)
    // s_forces_sum = s_forces + R * f_forces
    yarp::sig::Vector forces = s_sensorReadings.subVector(0, 2) + f_sensorReadings.subVector(0, 2);
    // s_moments_sum = s_moments + ( R * f_moments + cross( off, R * f_forces) )
    yarp::sig::Vector moments = f_sensorReadings.subVector(3, 5) + yarp::math::cross(fts_offset, f_sensorReadings.subVector(0, 2));
    moments += s_sensorReadings.subVector(3, 5);

    // express wrenches in the desired output SoR
    // right now assuming off == zero(3) -> cross( off, ~) == 0
    if (!(s_fts_to_out_R == eye(3)))
    {
        forces = s_fts_to_out_R * forces;
        moments = s_fts_to_out_R * moments;
    }
    out.setSubvector(0, forces);
    out.setSubvector(3, moments);

    // When you update the sensor readings, you also need to update the timestamp
    devout_timestamp.update((f_timestamp.getTime() + s_timestamp.getTime()) / 2.0);

    combineFtsStatus();
    return p_status;
}

void yarp::dev::ftshoeDriver::combineFtsStatus()
{
    if (f_status == AS_ERROR || s_status == AS_ERROR)
    {
        p_status = AS_ERROR;
    }
    else if (f_status == AS_TIMEOUT || s_status == AS_TIMEOUT)
    {
        p_status = AS_TIMEOUT;
    }
    else if (f_status == AS_OVF || s_status == AS_OVF)
    {
        p_status = AS_OVF;
    }
    else if (f_status == AS_OK && s_status == AS_OK)
    {
        p_status = AS_OK;
    }
    else
    {
        yError("ftShoeDriver: fts status have unexpected values %d, %d,\n.", f_status, s_status);
        p_status = AS_ERROR;
    }
}

int yarp::dev::ftshoeDriver::getState(int /*ch*/)
{
    yarp::os::LockGuard guard(p_mutex);

    return p_status;
}

int yarp::dev::ftshoeDriver::getChannels()
{
    return 6;
}

int yarp::dev::ftshoeDriver::calibrateSensor()
{
    yarp::os::LockGuard guard(p_mutex);
    yWarning("Calibration not yet implemented");
    return p_status;
}

int yarp::dev::ftshoeDriver::calibrateSensor(const yarp::sig::Vector& /*value*/)
{
    yarp::os::LockGuard guard(p_mutex);
    yWarning("Calibration not yet implemented");
    return p_status;
}

int yarp::dev::ftshoeDriver::calibrateChannel(int /*ch*/)
{
    yarp::os::LockGuard guard(p_mutex);
    yWarning("Calibration not yet implemented");
    return p_status;
}

int yarp::dev::ftshoeDriver::calibrateChannel(int /*ch*/, double /*v*/)
{
    yarp::os::LockGuard guard(p_mutex);
    yWarning("Calibration not yet implemented");
    return p_status;
}

yarp::os::Stamp yarp::dev::ftshoeDriver::getLastInputStamp()
{
    return devout_timestamp;
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


// IWrapper interface

bool yarp::dev::ftshoeDriver::attach(yarp::dev::PolyDriver *poly)
{
    // Disable attach function
    yError("ftShoeDriver: single device attach not supported. Please use attachAll.");
    return false;
}

bool yarp::dev::ftshoeDriver::detach()
{
    // Disable detach function, use detachAll instead
    yError("ftShoeDriver: single device detach not supported. Please use detachAll.");
    return false;
}

// IMultipleWrapper interface

bool yarp::dev::ftshoeDriver::attachAll(const yarp::dev::PolyDriverList &driverList)
{

    if (driverList.size() != 2)
    {
        yError("ftShoeDriver: cannot attach more or less than two devices");
        return false;
    }

    const yarp::dev::PolyDriverDescriptor *firstDriver = driverList[0];
    if (!firstDriver) {
        yError("Failed to get the driver descriptor");
        return false;
    }

    const yarp::dev::PolyDriverDescriptor *secondDriver = driverList[1];
    if (!secondDriver) {
        yError("Failed to get the driver descriptor");
        return false;
    }

    yarp::os::LockGuard guard(p_mutex);

    // attach the first ftSensor
    if (!firstDriver->poly || f_sensor_p) return false;
    if (!firstDriver->poly->view(f_sensor_p) || !f_sensor_p) return false;
    f_status = f_sensor_p->getChannels() > 0 ? AS_OK : AS_ERROR;

    // attach the second ftSensor
    if (!secondDriver->poly || s_sensor_p) return false;
    if (!secondDriver->poly->view(s_sensor_p) || !s_sensor_p) return false;
    s_status = s_sensor_p->getChannels() > 0 ? AS_OK : AS_ERROR;

    // return 1 if everything went fine with attachAll
    return !(f_status && s_status);
}

bool yarp::dev::ftshoeDriver::detachAll()
{
    yarp::os::LockGuard guard(p_mutex);

    // detach ftSensors
    f_sensor_p = 0;
    s_sensor_p = 0;

    // clear status variables
    s_status = AS_ERROR;
    f_status = AS_ERROR;
    p_status = AS_ERROR;

    return true;
}
