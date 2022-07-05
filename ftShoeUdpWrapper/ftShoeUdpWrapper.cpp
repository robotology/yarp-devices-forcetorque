/*
 * Copyright (C) 2018 iCub Facility
 * Authors: Diego Ferigo, Luca Tagliapietra
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "ftShoeUdpWrapper.h"

#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/PreciselyTimed.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Property.h>
#include <yarp/sig/Vector.h>

#include <string>

using namespace yarp::dev;

const unsigned messageBufferSize = 14; // 2*6 (wrenches) + 2 (absTime, relTime)
const unsigned default_thread_period = 10; // ms
const std::string logPrefix = "ftShoeUdpWrapper : ";

const unsigned AS_OK = yarp::dev::IAnalogSensor::AS_OK;
const unsigned AS_ERROR = yarp::dev::IAnalogSensor::AS_OK;
const unsigned AS_OVF = yarp::dev::IAnalogSensor::AS_OK;
const unsigned AS_TIMEOUT = yarp::dev::IAnalogSensor::AS_OK;

ftShoeUdpWrapper::ftShoeUdpWrapper()
    : PeriodicThread(default_thread_period)
    , m_1_timestamp(new yarp::os::Stamp())
    , m_2_timestamp(new yarp::os::Stamp())
    , m_initialTimestamp(0)
    , m_1_shoeTimestamps(nullptr)
    , m_2_shoeTimestamps(nullptr)
    , m_1_shoeSensor(nullptr)
    , m_2_shoeSensor(nullptr)
    , m_1_sensorData(new yarp::sig::Vector(6))
    , m_2_sensorData(new yarp::sig::Vector(6))
    , m_bufferForSerialization(messageBufferSize, 0)
    , m_socket(nullptr)
{
    m_1_sensorData->zero();
    m_2_sensorData->zero();
}

ftShoeUdpWrapper::~ftShoeUdpWrapper() {}

// ======================
// DeviceDriver interface
// ======================

bool ftShoeUdpWrapper::open(yarp::os::Searchable& config)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    yarp::os::Property prop;
    prop.fromString(config.toString().c_str());

    if (!(prop.check("endpointAddress") && prop.find("endpointAddress").isString())) {
        yError()
            << logPrefix
                   + "Endpoint IP address parameter missing or invalid in the configuration file";
        return false;
    }

    if (!(prop.check("udpPort") && prop.find("udpPort").isInt32())) {
        yError() << logPrefix + "UDP Port parameter missing or invalid in the configuration file";
        return false;
    }

    if (!(prop.check("threadPeriod") && prop.find("threadPeriod").isInt32())) {
        yError() << logPrefix
                        + "Thread period parameter missing or invalid in the configuration file";
        return false;
    }

    m_address = prop.find("endpointAddress").asString();
    m_port = static_cast<unsigned>(prop.find("udpPort").asInt32());
    const int threadPeriod = prop.find("threadPeriod").asInt32();

    if (!setPeriod(threadPeriod)) {
        yError() << logPrefix + "Failed to set specified thread period";
        return false;
    }

    yInfo() << logPrefix + "Publishing on " + m_address + ":" + std::to_string(m_port)
            << " every " + std::to_string(threadPeriod) + " ms";

    m_socket.reset(new asio::ip::udp::socket(m_io_service));
    asio::error_code err;
    m_endpoint = asio::ip::udp::endpoint(asio::ip::address_v4::from_string(m_address), m_port);
    m_socket->open(asio::ip::udp::v4(), err);

    // the internal operator ! has been overridden by asio to parse the error code
    if (!(!err)) {
        yError() << logPrefix + "Failed to open udp socket";
        return false;
    }

    return true;
}

bool ftShoeUdpWrapper::close()
{
    m_socket->close();
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
        std::lock_guard<std::mutex> guard(m_mutex);

        // Attach the first shoe
        if (!firstDriver->poly || m_1_shoeSensor)
            return false;
        if (!firstDriver->poly->view(m_1_shoeSensor) || !m_1_shoeSensor)
            return false;
        if (!firstDriver->poly->view(m_1_shoeTimestamps) || !m_1_shoeTimestamps)
            return false;
        m_1_ok = m_1_shoeSensor->getChannels() > 0 ? AS_OK : AS_ERROR;

        // Attach the second shoe
        if (!secondDriver->poly || m_2_shoeSensor)
            return false;
        if (!secondDriver->poly->view(m_2_shoeSensor) || !m_2_shoeSensor)
            return false;
        if (!secondDriver->poly->view(m_2_shoeTimestamps) || !m_2_shoeTimestamps)
            return false;
        m_2_ok = m_2_shoeSensor->getChannels() > 0 ? AS_OK : AS_ERROR;
    }

    // Start the publisher thread
    if (!start()) {
        yError() << logPrefix + "Failed to start the publisher thread";
        return false;
    }

    // Return 1 if everything went fine with attachAll
    return !(m_1_ok && m_2_ok);
}

bool ftShoeUdpWrapper::detachAll()
{
    std::lock_guard<std::mutex> guard(m_mutex);

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
// PeriodicThread class
// ================

void ftShoeUdpWrapper::run()
{
    // Read the data
    // -------------

    double avgTimestamp;

    {
        std::lock_guard<std::mutex> guard(m_mutex);

        // Read the timestamps
        *m_1_timestamp = m_1_shoeTimestamps->getLastInputStamp();
        *m_2_timestamp = m_2_shoeTimestamps->getLastInputStamp();

        avgTimestamp = 0.5 * (m_1_timestamp->getTime() + m_2_timestamp->getTime());

        // Read the first shoe data
        if (m_1_shoeSensor->read(*m_1_sensorData) != AS_OK) {
            yError() << logPrefix + "Failed to read first shoe data";
        }

        // Read the second shoe data
        if (m_2_shoeSensor->read(*m_2_sensorData) != AS_OK) {
            yError() << logPrefix + "Failed to read second shoe data";
        }
    }

    if (m_initialTimestamp == 0) {
        m_initialTimestamp = avgTimestamp;
    }

    double relativeTimestamp = avgTimestamp - m_initialTimestamp;

    // Pack data for serialization
    // ---------------------------
    // Serialization Protocol: Right Shoe Wrenches, Left Shoe Wrenches, Absolute Time, Relative Time
    m_bufferForSerialization.back() = static_cast<float>(relativeTimestamp);
    m_bufferForSerialization.at(messageBufferSize - 2) = static_cast<float>(avgTimestamp);

    for (size_t i = 0; i < (*m_1_sensorData).size(); ++i) {
        m_bufferForSerialization[i] = static_cast<float>((*m_1_sensorData)[i]);
        m_bufferForSerialization[i + (*m_1_sensorData).size()]
            = static_cast<float>((*m_2_sensorData)[i]);
    }

    // Forward the data through UDP
    // ----------------------------
    asio::error_code err;

    // Firstly, check if the socket is still open
    if (!m_socket->is_open()) {
        yWarning() << logPrefix + "The socket was closed externally. Trying to reopen it";
        m_socket->open(asio::ip::udp::v4(), err);

        if (!err) {
            yError() << logPrefix + "Failed to open udp socket";
            return;
        }
    }

    // Then, send the UDP message
    auto sentMessageSize
        = m_socket->send_to(asio::buffer(m_bufferForSerialization.data(),
                                         m_bufferForSerialization.size() * sizeof(float)),
                            m_endpoint);
}
