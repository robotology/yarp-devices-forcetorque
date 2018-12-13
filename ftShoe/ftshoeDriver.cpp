/*
 * Copyright (C) 2017 iCub Facility
 * Authors: Luca Tagliapietra, Silvio Traversaro
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
#include <thread>
#include <chrono>


yarp::dev::ftshoeDriver::ftshoeDriver() : f_sensorReadings(6),
                                          s_sensorReadings(6),
                                          devout_data(6),
                                          f_status(yarp::dev::IAnalogSensor::AS_OK),
                                          s_status(yarp::dev::IAnalogSensor::AS_OK),
                                          p_status(yarp::dev::IAnalogSensor::AS_OK),
                                          f_sensor_p(0),
                                          s_sensor_p(0),
                                          fts_offset(3),
                                          fts_orientation_R(3,3),
                                          s_fts_to_out_R(3,3),
                                          static_offsets(6),
                                          f_insitu_matrix(6,6),
                                          s_insitu_matrix(6,6)
{
    // Initialize input buffers with zero values
    f_sensorReadings.zero();
    s_sensorReadings.zero();

    // initialize output data buffer
    devout_data.zero();

    // initialize ftShoe settings
    fts_offset.zero();
    fts_orientation_R.zero();
    s_fts_to_out_R.zero();

    // initialize to false the calibration
    static_offsets.zero();
    calibrated = false;

    // initialize to false use of inSitu calibration matrices
    useInSituCalibration = false;
    f_insitu_matrix.zero();
    s_insitu_matrix.zero();

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

    if (!prop.check("fts_orientation_R"))
    {
        yWarning() << "ftshoeDriver : fts orientation matrix non found, assuming they are aligned so, parameter set to identity";
        fts_orientation_R = eye(3);
    }
    else
    {
        if (prop.find("fts_orientation_R").isList() && prop.find("fts_orientation_R").asList()->size() == 9)
        {
            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int rowMajorIndex = 3 * r + c;
                    fts_orientation_R(r, c) = prop.find("fts_orientation_R").asList()->get(rowMajorIndex).asDouble();
                }
            }
        }
        else
        {
            yError() << "ftshoeDriver : fts orientation matrix wrongly defined";
            return false;
        }
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

    if (!prop.check("useInSituCalibration"))
    {
        yWarning() << "ftshoeDriver : useInSituCalibration parameter not found. Use default workbench calibration.";
        useInSituCalibration = false;
    }
    else if (prop.find("useInSituCalibration").isBool())
    {
        useInSituCalibration = prop.find("useInSituCalibration").asBool();
    }
    else
    {
        yWarning() << "ftshoeDriver : useInSituCalibration parameter found but wrongly defined.";
        return false;
    }

    if (useInSituCalibration)
    {
        if (prop.findGroup("inSituMatrices").isNull() || !prop.findGroup("inSituMatrices").check("front_fts") || !prop.findGroup("inSituMatrices").check("rear_fts"))
        {
            yError() << "ftshoeDriver: inSituMatrices parameters group not defined. Aborting." << prop.findGroup("inSituMatrices").isNull();
            return false;
        }
        else
        {
            const yarp::os::Bottle group = prop.findGroup("inSituMatrices");
            if (!group.find("front_fts").isList() || !group.find("rear_fts").isList() || group.find("front_fts").asList()->size() != 36 || group.find("rear_fts").asList()->size() != 36)
            {
                yError() << "ftshoeDriver: inSituMatrices parameters found but wrongly formatted.";
                return false;
            }
            else
            {
                for (int r = 0; r < 6; r++)
                {
                    for (int c = 0; c < 6; c++)
                    {
                        int rowMajorIndex = 6 * r + c;
                        f_insitu_matrix(r, c) = group.find("front_fts").asList()->get(rowMajorIndex).asDouble();
                        s_insitu_matrix(r, c) = group.find("rear_fts").asList()->get(rowMajorIndex).asDouble();
                    }
                }
            }
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

    // Change sign to obtain the wrenches exerted by the human on the fts,
    // while the fts measure the vice versa
    f_sensorReadings = -1 * f_sensorReadings;
    s_sensorReadings = -1 * s_sensorReadings;

    // if using inSitu Calibration results, first of all get re-calibrated data
    if (useInSituCalibration)
    {
        f_sensorReadings = f_insitu_matrix * f_sensorReadings;
        s_sensorReadings = s_insitu_matrix * s_sensorReadings;
    }

    // move data in first fts SoR to second fts SoR
    // right now assuming fts SoRs are aligned R == eye(3)
    // s_forces_sum = s_forces + R * f_forces
    yarp::sig::Vector forces = s_sensorReadings.subVector(0, 2) + fts_orientation_R*f_sensorReadings.subVector(0, 2);
    // s_moments_sum = s_moments + ( R * f_moments + cross( off, R * f_forces) )
    yarp::sig::Vector moments = s_sensorReadings.subVector(3, 5) + yarp::math::cross(fts_offset, fts_orientation_R*f_sensorReadings.subVector(0, 2));
    moments += fts_orientation_R*f_sensorReadings.subVector(3, 5);

    // express wrenches in the desired output SoR
    // right now assuming off == zero(3) -> cross( off, ~) == 0
    if (!(s_fts_to_out_R == eye(3)))
    {
        forces = s_fts_to_out_R * forces;
        moments = s_fts_to_out_R * moments;
    }
    out.setSubvector(0, forces);
    out.setSubvector(3, moments);

    if (calibrated)
    {
        out -= static_offsets;
    }
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
    // Calibration procedure to remove static offset
    yInfo() << "Starting calibration";
    yInfo() << "Please hold the ftShoe horizontal without touching the sole for 5 seconds";
    yarp::sig::Vector calSample(6);
    yarp::sig::Vector tmpOffsets(6);

    calSample.zero();
    tmpOffsets.zero();

    // Reset calibration data to support consecutive ftshoe recalibrations
    {
        yarp::os::LockGuard guard(p_mutex);
        calibrated = false;
        static_offsets.zero();
    }

    int status;

    status = read(tmpOffsets);
    if (status != AS_OK)
    {
        yError("Unable to read data. Aborting. Please try againg");
        return AS_ERROR;
    }

    // Average data for 5 sec to estimate static offsets
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    int nSample = 0;
    while (std::chrono::steady_clock::now() < start + std::chrono::milliseconds(5000))
    {
        status = read(calSample);
        if (status == AS_OK)
        {
            ++nSample;
            tmpOffsets = (tmpOffsets + calSample) / 2.0;
        }
        else
        {
            yWarning("Unable to read data from fts, skipping calibration sample");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    yInfo() << "Processing..." << nSample << " total samples";

    yarp::os::LockGuard guard(p_mutex);
    static_offsets = tmpOffsets;
    calibrated = true;
    yInfo() << "Calibration successful.";
    return p_status;
}

int yarp::dev::ftshoeDriver::calibrateSensor(const yarp::sig::Vector& /*value*/)
{
    yWarning("ftshoeDriver: calibrateSensor not supported");
    return AS_ERROR;
}

int yarp::dev::ftshoeDriver::calibrateChannel(int /*ch*/)
{
    yWarning("ftshoeDriver: calibrateChannel not supported");
    return AS_ERROR;
}

int yarp::dev::ftshoeDriver::calibrateChannel(int /*ch*/, double /*v*/)
{
    yWarning("ftshoeDriver: calibrateChannel not supported");
    return AS_ERROR;
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
