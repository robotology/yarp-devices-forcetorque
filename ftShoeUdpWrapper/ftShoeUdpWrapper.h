/*
 * Copyright (C) 2018 iCub Facility
 * Authors: Diego Ferigo, Luca Tagliapietra
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#ifndef YARP_ftShoeUdpWrapper_H
#define YARP_ftShoeUdpWrapper_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/Wrapper.h>

#include <yarp/os/PeriodicThread.h>

#include <yarp/sig/Vector.h>

#include <iostream>
#include <memory>
#include <stdio.h>
#include <vector>
#include <mutex>

#include <asio.hpp>

namespace yarp {
namespace dev {
class IAnalogSensor;
class IPreciselyTimed;
class ftShoeUdpWrapper;
} // namespace dev
namespace os {
class Stamp;
}
} // namespace yarp

class yarp::dev::ftShoeUdpWrapper final : public yarp::dev::DeviceDriver,
                                          public yarp::dev::IMultipleWrapper,
                                          public yarp::os::PeriodicThread {
private:
    // Mutex to avoid race conditions
    std::mutex m_mutex;

    // Containers where to store timestamps
    std::unique_ptr<yarp::os::Stamp> m_1_timestamp;
    std::unique_ptr<yarp::os::Stamp> m_2_timestamp;
    double m_initialTimestamp;

    // Interfaces to read timestamps
    yarp::dev::IPreciselyTimed* m_1_shoeTimestamps;
    yarp::dev::IPreciselyTimed* m_2_shoeTimestamps;

    // Interfaces to read sensor data
    yarp::dev::IAnalogSensor* m_1_shoeSensor;
    yarp::dev::IAnalogSensor* m_2_shoeSensor;

    // Storage for sensor data
    std::unique_ptr<yarp::sig::Vector> m_1_sensorData;
    std::unique_ptr<yarp::sig::Vector> m_2_sensorData;

    // Storage for parameter
    std::string m_address;
    unsigned m_port;

    // Storage for serialized data
    std::vector<float> m_bufferForSerialization;

    // Shoe readings states
    int m_1_ok;
    int m_2_ok;

    // UDP socket related
    asio::io_service m_io_service;
    asio::ip::udp::endpoint m_endpoint;
    std::unique_ptr<asio::ip::udp::socket> m_socket;

public:
    ftShoeUdpWrapper();
    ~ftShoeUdpWrapper() override;

    // Prevent copy
    ftShoeUdpWrapper(const ftShoeUdpWrapper& other) = delete;
    ftShoeUdpWrapper& operator=(const ftShoeUdpWrapper& other) = delete;

    // DeviceDriver interface
    bool open(yarp::os::Searchable& config) override;
    bool close() override;

    // IMultipleWrapper interface
    bool attachAll(const PolyDriverList& devices2Attach) override;
    bool detachAll() override;

    // PeriodicThread class
    void run() override;
};

#endif
