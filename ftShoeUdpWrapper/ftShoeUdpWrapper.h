/*
 * Copyright (C) 2018 iCub Facility
 * Authors: Diego Ferigo
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#ifndef YARP_ftShoeUdpWrapper_H
#define YARP_ftShoeUdpWrapper_H

#include <yarp/dev/Wrapper.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/DeviceDriver.h>

#include <yarp/os/Mutex.h>
#include <yarp/os/RateThread.h>

#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>

namespace yarp {
namespace dev {
class IAnalogSensor;
class IPreciselyTimed;
class ftShoeUdpWrapper;
}
namespace os {
class Stamp;
}
namespace sig {
class Vector;
}
}

class yarp::dev::ftShoeUdpWrapper final : public yarp::dev::DeviceDriver,
                                          public yarp::dev::IMultipleWrapper,
                                          public yarp::os::RateThread
{
private:
    // Mutex to avoid race conditions
    yarp::os::Mutex m_mutex;

    // Containers where to store timestamps
    std::unique_ptr<yarp::os::Stamp> m_1_timestamp;
    std::unique_ptr<yarp::os::Stamp> m_2_timestamp;

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
    std::vector<double> m_bufferForSerialization;

    // Other
    int m_1_ok;
    int m_2_ok;

public:
    ftShoeUdpWrapper();
    ~ftShoeUdpWrapper() override;

    // Prevent copy
    ftShoeUdpWrapper(const ftShoeUdpWrapper& other) = delete;
    ftShoeUdpWrapper & operator=(const ftShoeUdpWrapper& other) = delete;

    // DeviceDriver interface
    bool open(yarp::os::Searchable& config) override;
    bool close() override;

    // IMultipleWrapper interface
    bool attachAll(const PolyDriverList& devices2Attach) override;
    bool detachAll() override;

    // RateThread class
    void run() override;
};

#endif
