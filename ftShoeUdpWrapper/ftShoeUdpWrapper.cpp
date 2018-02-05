/*
 * Copyright (C) 2018 iCub Facility
 * Authors: Diego Ferigo
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "ftShoeUdpWrapper.h"

#include <yarp/sig/Vector.h>
#include <yarp/os/Property.h>
#include <yarp/os/LockGuard.h>
#include <yarp/os/LogStream.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/PreciselyTimed.h>

#include <string>
#include <asio.hpp>

using namespace yarp::dev;

const unsigned default_thread_period = 10; // ms
const std::string logPrefix = "ftShoeUdpWrapper : ";

const unsigned AS_OK = yarp::dev::IAnalogSensor::AS_OK;
const unsigned AS_ERROR = yarp::dev::IAnalogSensor::AS_OK;
const unsigned AS_OVF = yarp::dev::IAnalogSensor::AS_OK;
const unsigned AS_TIMEOUT = yarp::dev::IAnalogSensor::AS_OK;

ftShoeUdpWrapper::ftShoeUdpWrapper()
    : RateThread(default_thread_period)
    , m_1_timestamp(new yarp::os::Stamp())
    , m_2_timestamp(new yarp::os::Stamp())
    , m_1_shoeTimestamps(nullptr)
    , m_2_shoeTimestamps(nullptr)
    , m_1_shoeSensor(nullptr)
    , m_2_shoeSensor(nullptr)
    , m_1_sensorData(new yarp::sig::Vector(6))
    , m_2_sensorData(new yarp::sig::Vector(6))
    , m_bufferForSerialization(13, 0)
{
    m_1_sensorData->zero();
    m_2_sensorData->zero();
}

ftShoeUdpWrapper::~ftShoeUdpWrapper()
{}

// ======================
// DeviceDriver interface
// ======================

bool ftShoeUdpWrapper::open(yarp::os::Searchable& config)
{
    yarp::os::LockGuard guard(m_mutex);

    yarp::os::Property prop;
    prop.fromString(config.toString().c_str());

    if (!(prop.check("address") && prop.find("address").isString())) {
        yError() << logPrefix + "Address parameter missing or invalid in the configuration file";
        return false;
    }

    if (!(prop.check("udpPort") && prop.find("udpPort").isInt())) {
        yError() << logPrefix + "UDP Port parameter missing or invalid in the configuration file";
        return false;
    }

    if (!(prop.check("threadPeriod") && prop.find("threadPeriod").isInt())) {
        yError() << logPrefix + "Thread period parameter missing or invalid in the configuration file";
        return false;
    }

    m_address = prop.find("address").asString();
    m_port = static_cast<unsigned>(prop.find("udpPort").asInt());
    const int threadPeriod = prop.find("threadPeriod").asInt();

    if (!setRate(threadPeriod)) {
        yError() << logPrefix + "Failed to set specified thread period";
        return false;
    }

    yInfo() << logPrefix + "Publishing on " + m_address + ":" + std::to_string(m_port)
            << " every " + std::to_string(threadPeriod) + " ms";

    return true;
}

bool ftShoeUdpWrapper::close()
{
    return true;
}

// ==========================
// IMultipleWrapper interface
// ==========================

bool ftShoeUdpWrapper::attachAll(const yarp::dev::PolyDriverList& driverList)
{
    if (driverList.size() != 2) {
        yError() << logPrefix + "Cannot attach more or less than two devices";
        return false;
    }

    const yarp::dev::PolyDriverDescriptor* firstDriver = driverList[0];
    if (!firstDriver) {
        yError() << logPrefix + "Failed to get the first driver descriptor";
        return false;
    }

    const yarp::dev::PolyDriverDescriptor* secondDriver = driverList[1];
    if (!secondDriver) {
        yError() << logPrefix + "Failed to get the second driver descriptor";
        return false;
    }

    {
        yarp::os::LockGuard guard(m_mutex);

        // Attach the first shoe
        if (!firstDriver->poly || m_1_shoeSensor) return false;
        if (!firstDriver->poly->view(m_1_shoeSensor) || !m_1_shoeSensor) return false;
        if (!firstDriver->poly->view(m_1_shoeTimestamps) || !m_1_shoeTimestamps) return false;
        m_1_ok = m_1_shoeSensor->getChannels() > 0 ? AS_OK : AS_ERROR;

        // Attach the second shoe
        if (!secondDriver->poly || m_2_shoeSensor) return false;
        if (!secondDriver->poly->view(m_2_shoeSensor) || !m_2_shoeSensor) return false;
        if (!secondDriver->poly->view(m_2_shoeTimestamps) || !m_2_shoeTimestamps) return false;
        m_2_ok = m_2_shoeSensor->getChannels() > 0 ? AS_OK : AS_ERROR;
    }

    // Return 1 if everything went fine with attachAll
    return !(m_1_ok && m_2_ok);
}

bool ftShoeUdpWrapper::detachAll()
{
    yarp::os::LockGuard guard(m_mutex);

    // Detach the shoes sensor interface
    m_1_shoeSensor = nullptr;
    m_2_shoeSensor = nullptr;

    // Detach the shoes timestamp interface
    m_1_shoeTimestamps = nullptr;
    m_1_shoeTimestamps = nullptr;

    // Clear status variables
    m_1_ok = AS_ERROR;
    m_2_ok = AS_ERROR;

    return true;
}

// ================
// RateThread class
// ================

void ftShoeUdpWrapper::run()
{
    // Read the data
    // -------------

    double avgTimestamp;

    {
        yarp::os::LockGuard guard(m_mutex);

        // Read the timestamps
        *m_1_timestamp = m_1_shoeTimestamps->getLastInputStamp();
        *m_2_timestamp = m_2_shoeTimestamps->getLastInputStamp();

        avgTimestamp = 0.5 * (m_1_timestamp->getTime() + m_2_timestamp->getTime());

        // Read the first shoe data
        if (!m_1_shoeSensor->read(*m_1_sensorData)) {
            yError() << logPrefix + "Failed to read first shoe data";
        }

        // Read the second shoe data
        if (!m_2_shoeSensor->read(*m_2_sensorData)) {
            yError() << logPrefix + "Failed to read second shoe data";
        }
    }

    // Pack data for serialization
    // ---------------------------

    m_bufferForSerialization.back() = avgTimestamp;

    for (size_t i = 0; i < 6; ++i) {
        m_bufferForSerialization[i] = (*m_1_sensorData)[i];
        m_bufferForSerialization[i + 6] = (*m_2_sensorData)[i];
    }

    // Forward the data through UDP
    // ----------------------------

    asio::io_service io_service;
    asio::ip::udp::resolver resolver(io_service);

    // Generate the endpoint from address and port
    asio::ip::udp::resolver::query query(asio::ip::udp::v4(),
                                         m_address,
                                         std::to_string(m_port));
    asio::ip::udp::endpoint endpoint;
    endpoint = *(resolver.resolve(query));

    // Open the socket
    asio::ip::udp::socket socket(io_service);

    // Forward data
    socket.send_to(asio::buffer(m_bufferForSerialization.data(),
                                m_bufferForSerialization.size()), endpoint);
}
