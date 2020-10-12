/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#include "AMTIForcePlate.h"

#include "IMultipleForcePlates.h"
#include <yarp/os/LockGuard.h>
#include <yarp/os/LogStream.h>
#include <cassert>

#include <iDynTree/Core/EigenHelpers.h>
#include <iDynTree/yarp/YARPConversions.h>
#include <iDynTree/Core/Wrench.h>

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
    yarp::os::LockGuard guard(m_mutex);
    if (!config.check("platformID", "Looking for platform ID")) {
        yError("platformID not found in the configuration");
        return false;
    }

    m_platformID = config.find("platformID").toString();
    if (m_platformID.empty()) {
        yError("platformID not found in the configuration");
        return false;
    }
    yInfo() << "PlatformID " << config.find("platformID").asString();


    // Get platform rotation
    m_rotation_angle = 0.0;
    m_transform = iDynTree::Transform::Identity();
    iDynTree::Rotation rotz = iDynTree::Rotation::Identity();
    if (!config.check("platformRotation") )
    {
        yWarning("<platformRotations> configuration parameter is not passed. Using default rotation 0.0");
    }
    else
    {
        m_rotation_angle = - config.find("platformRotation").asFloat64();
        rotz = iDynTree::Rotation::RotZ(m_rotation_angle);
        yInfo("Using platform rotation of %f", m_rotation_angle);
    }

    // Set transform with rotation
    m_transform.setRotation(rotz);

    yInfo("m_transform is %s", m_transform.toString().c_str());

    return true;

}
bool yarp::dev::AMTIForcePlate::close()
{
    yarp::os::LockGuard guard(m_mutex);
    m_platformDriver = 0;
    return true;
}

// IAnalogSensor interface
int yarp::dev::AMTIForcePlate::read(yarp::sig::Vector &out)
{
    yarp::os::LockGuard guard(m_mutex);
    if (!m_platformDriver) return AS_ERROR;
    m_status = m_platformDriver->getLastMeasurementForPlateAtIndex(m_platformIndex,
                                                                   m_sensorReadings,
                                                                   &m_timestamp);
    // Transform wrench measurements
    iDynTree::Wrench inputWrench;
    inputWrench.zero();
    iDynTree::toiDynTree(m_sensorReadings, inputWrench);

    Eigen::Matrix<double, 6, 1> transformedWrenchEigen;
    transformedWrenchEigen.Zero();
    iDynTree::Wrench outputWrench;
    outputWrench.zero();

    transformedWrenchEigen =  iDynTree::toEigen(m_transform.asAdjointTransformWrench()) * iDynTree::toEigen(inputWrench);
    iDynTree::fromEigen(outputWrench, transformedWrenchEigen);
    iDynTree::toYarp(outputWrench, out);

    yInfo("PlatformID %d", m_platformIndex);
    yInfo("Input Wrench %s", m_sensorReadings.toString().c_str());
    yInfo("Output Wrench %s", out.toString().c_str());

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
    yarp::os::LockGuard guard(m_mutex);
    return m_timestamp;
}

// IWrapper interface
bool yarp::dev::AMTIForcePlate::attach(yarp::dev::PolyDriver *poly)
{
    yarp::os::LockGuard guard(m_mutex);
    if (!poly || m_platformDriver) return false;
    if (!poly->view(m_platformDriver) || !m_platformDriver) return false;
    m_platformIndex = m_platformDriver->getPlatformIndexForPlatformID(m_platformID);
    yInfo("Platform with ID %s associated to index %d", m_platformID.c_str(), m_platformIndex);

    m_status = m_platformIndex >= 0 ? AS_OK : AS_ERROR;
    return m_platformIndex >= 0;
}

bool yarp::dev::AMTIForcePlate::detach()
{
    yarp::os::LockGuard guard(m_mutex);
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
