/*
 * Copyright (C) 2019 iCub Facility
 * Authors: Yeshasvi Tirupachuri
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "ftnodeDriver.h"

#include <yarp/os/LogStream.h>

#include <mutex>
#include <string>
#include <stdio.h>
#include <vector>

const std::string DeviceName = "ftnodeDriver";
const std::string LogPrefix = DeviceName + ":";
double period = 0.01;

using namespace yarp::dev;
using namespace yarp::os;

struct AnalogSensorData
{
    size_t numberOfChannels = 0;
    std::vector<double> measurements;
};

class ftnodeDriver::Impl
{
public:
    mutable std::recursive_mutex mutex;
    yarp::dev::ISerialDevice *iSerialDevice = nullptr;

    AnalogSensorData analogSensorData;
};


// Default constructor
ftnodeDriver::ftnodeDriver()
    : PeriodicThread(period)
    , pImpl{new Impl()}
{}

// Destructor
ftnodeDriver::~ftnodeDriver() = default;

bool ftnodeDriver::open(yarp::os::Searchable& config)
{
    // Check the configuration parameters

    // Period of the this device
    if (!(config.check("period") && config.find("period").isFloat64())) {
        yInfo() << LogPrefix << "Using default period: " << period << "s";
    }
    else {
        period = config.find("period").asFloat64();
        yInfo() << LogPrefix << "Using the period : " << period << "s";
    }

    // The number of channels to be configured for IAnalogSensor interface
    // Currently, the serial port streams data from only one FT and in the
    // future it will also stream the data from the four FTs of the FTshoes
    if (!(config.check("channels") && config.find("channels").isInt())) {
        yError() << LogPrefix << "Option 'channels' not found or not a valid integer";
        return false;
    }

    // Parse the configuration parameters

    pImpl->analogSensorData.numberOfChannels = config.find("channels").asInt();

    // Resize the measurements buffer and initialize to zero
    pImpl->analogSensorData.measurements.resize(pImpl->analogSensorData.numberOfChannels * 6, 0.0);

    return true;

}

void ftnodeDriver::run()
{
    //TODO
    yInfo() << LogPrefix << "Inside run";
}

bool ftnodeDriver::close()
{
    detach();
    return true;
}

bool ftnodeDriver::attach(yarp::dev::PolyDriver* poly)
{
    if (!poly) {
        yError() << LogPrefix << "Passed PolyDriver is a nullptr";
        return false;
    }

    // TODO: Check for ISerialDevice interface
    if (pImpl->iSerialDevice || !poly->view(pImpl->iSerialDevice) || !pImpl->iSerialDevice) {
        yError() << LogPrefix << "Failed to view the ISerialDevice interface from the attached polydriver device";
        return false;
    }
    else {
        yInfo() << LogPrefix << "ISerialDevice interface viewed correctly";
    }

    // TODO: Check if the ISerialDevice interface is configured correctly
    // I do not see any method to check this

    // Start the PeriodicThread loop
    if (!start()) {
        yError() << LogPrefix << "Failed to start the period thread.";
        return false;
    }

    yInfo() << LogPrefix << "attach() successful";
    return true;

}

bool ftnodeDriver::detach()
{
    pImpl->iSerialDevice = nullptr;
    return true;
}

bool ftnodeDriver::attachAll(const yarp::dev::PolyDriverList& driverList)
{
    // TODO: Check how many serial devices are needed for the ft wireless sheos
    if (driverList.size() > 1) {
        yError() << LogPrefix << "This wrapper accepts only one attached yarp Serial device";
        return false;
    }

    const yarp::dev::PolyDriverDescriptor* driver = driverList[0];

    if (!driver) {
        yError() << LogPrefix << "Passed PolyDriverDescriptor is nullptr";
        return false;
    }

    return attach(driver->poly);
}

bool ftnodeDriver::detachAll()
{
    // TODO: Update in case of multiple serial devices are attached
    return detach();
}

void ftnodeDriver::threadRelease()
{}
