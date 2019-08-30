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

const std::string DeviceName = "ftnodeDriver";
const std::string LogPrefix = DeviceName + ":";

using namespace yarp::dev;
using namespace yarp::os;

class ftnodeDriver::Impl
{
public:
    mutable std::recursive_mutex mutex;
    yarp::dev::ISerialDevice *iSerialDevice = nullptr;
};


// Default constructor
ftnodeDriver::ftnodeDriver()
    : pImpl{new Impl()}
{}

// Destructor
ftnodeDriver::~ftnodeDriver() = default;

bool ftnodeDriver::open(yarp::os::Searchable& config)
{
    // TODO
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
}

bool ftnodeDriver::detach()
{
    // TODO Set interfaces to nullptr
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
