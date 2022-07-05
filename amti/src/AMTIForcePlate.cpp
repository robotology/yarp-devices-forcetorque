/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#include "AMTIForcePlate.h"

#include "IMultipleForcePlates.h"
#include <yarp/os/LogStream.h>
#include <yarp/sig/Vector.h>
#include <yarp/math/Math.h>

#include <cassert>
#include <cmath>

yarp::dev::AMTIForcePlate::AMTIForcePlate(const yarp::dev::AMTIForcePlate &other) { assert(false);  }
yarp::dev::AMTIForcePlate& yarp::dev::AMTIForcePlate::operator = (const yarp::dev::AMTIForcePlate &other) { assert(false); return *this; }

yarp::dev::AMTIForcePlate::AMTIForcePlate()
    : m_sensorReadings(6)
    , m_status(AS_ERROR)
    , m_platformDriver(0)
    , m_platformIndex(-1)
{}
yarp::dev::AMTIForcePlate::~AMTIForcePlate()
{}

// DeviceDriver interface
bool yarp::dev::AMTIForcePlate::open(yarp::os::Searchable &config)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if (!config.check("platformID", "Looking for platform ID")) {
        yError("platformID not found in the configuration");
        return false;
    }

    m_platformID = config.find("platformID").toString();
    if (m_platformID.empty()) {
        yError("platformID not found in the configuration");
        return false;
    }
    yInfo() << "Platform with ID " << m_platformID << " found";


    // Get platform rotation
    m_rotation_angle = 0.0;
    yarp::sig::Matrix m_transform;
    m_transform.zero();

    // Only z-axis rotation is considered as a configuration parameter
    // Reference: Check fmSetPlatformRotation() from http://www.amti.jp/Gen%205%20Programmers%20Reference.pdf
    if (!config.check("platformZRotation") )
    {
        yInfo("<platformZRotation> configuration parameter is not passed. Using default rotation 0 degree about z-axis for platform %s", m_platformID);
    }
    else
    {
        m_rotation_angle = config.find("platformZRotation").asFloat64();
        yInfo("Using platform rotation of %f degree about z-axis for platform %s", m_rotation_angle, m_platformID);
    }

    // Set transform with rotation
    yarp::sig::Vector rotz = {0, 0, (m_rotation_angle / 180) * M_PI};

    yarp::sig::Matrix transform = yarp::math::euler2dcm(rotz);
    yarp::sig::Matrix adjInv = yarp::math::adjointInv(transform);
    m_transform_wrench.zero();
    m_transform_wrench = adjInv.transposed();

    return true;

}
bool yarp::dev::AMTIForcePlate::close()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_platformDriver = 0;
    return true;
}

// IAnalogSensor interface
int yarp::dev::AMTIForcePlate::read(yarp::sig::Vector &out)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if (!m_platformDriver) return AS_ERROR;
    m_status = m_platformDriver->getLastMeasurementForPlateAtIndex(m_platformIndex,
                                                                   m_sensorReadings,
                                                                   &m_timestamp);
    // Transform wrench measurements
    out = m_transform_wrench * m_sensorReadings;
    return m_status;

}

int yarp::dev::AMTIForcePlate::getState(int ch) { return m_status; }
int yarp::dev::AMTIForcePlate::getChannels() { return m_status == AS_OK ? 6 : 0;  }
int yarp::dev::AMTIForcePlate::calibrateSensor() { return m_status;  }
int yarp::dev::AMTIForcePlate::calibrateSensor(const yarp::sig::Vector &value) { return m_status; }
int yarp::dev::AMTIForcePlate::calibrateChannel(int ch) { return m_status; }
int yarp::dev::AMTIForcePlate::calibrateChannel(int ch, double value) { return m_status; }

// IPreciselyTimed interface
yarp::os::Stamp yarp::dev::AMTIForcePlate::getLastInputStamp()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_timestamp;
}

// IWrapper interface
bool yarp::dev::AMTIForcePlate::attach(yarp::dev::PolyDriver *poly)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if (!poly || m_platformDriver) return false;
    if (!poly->view(m_platformDriver) || !m_platformDriver) return false;
    m_platformIndex = m_platformDriver->getPlatformIndexForPlatformID(m_platformID);
    yInfo("Platform with ID %s associated to index %d", m_platformID.c_str(), m_platformIndex);

    m_status = m_platformIndex >= 0 ? AS_OK : AS_ERROR;
    return m_platformIndex >= 0;
}

bool yarp::dev::AMTIForcePlate::detach()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_platformDriver = 0;
    m_status = AS_ERROR;
    return true;
}

bool yarp::dev::AMTIForcePlate::attachAll(const yarp::dev::PolyDriverList &driverList)
{
    if (driverList.size() > 1) {
        yError("Only one device to be attached is supported");
        return false;
    }
    const yarp::dev::PolyDriverDescriptor *firstDriver = driverList[0];
    if (!firstDriver) {
        yError("Failed to get the driver descriptor");
        return false;
    }
    return attach(firstDriver->poly);
}

bool yarp::dev::AMTIForcePlate::detachAll()
{
    return detach();
}
