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
    mutable std::mutex mutex;
    yarp::dev::ISerialDevice *iSerialDevice = nullptr;

    int numberOfFTSensors;
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

    // The number of sensors to be configured for IAnalogSensor interface
    // Currently, the serial port streams data from only one FT and in the
    // future it will also stream the data from the four FTs of the FTshoes
    if (!(config.check("numberOfFTSensors") && config.find("numberOfFTSensors").isInt())) {
        yError() << LogPrefix << "Option 'channels' not found or not a valid integer";
        return false;
    }

    // Parse the configuration parameters

    pImpl->numberOfFTSensors = config.find("numberOfFTSensors").asInt();

    // Set the number of channels, 6 for each FT sensor
    pImpl->analogSensorData.numberOfChannels = pImpl->numberOfFTSensors * 6;


    // Resize the measurements buffer and initialize to zero
    pImpl->analogSensorData.measurements.resize(pImpl->analogSensorData.numberOfChannels, 0.0);

    return true;

}

void ftnodeDriver::run()
{
    //TODO - Process data from the serial port
    yInfo() << LogPrefix << "Inside run";

    for(size_t i = 0; i < pImpl->numberOfFTSensors; i++) {

        // Expose the data as IAnalogSensor
        // ================================
        {
            std::lock_guard<std::mutex> lock(pImpl->mutex);

            pImpl->analogSensorData.measurements[6 * i + 0] = 0.0;
            pImpl->analogSensorData.measurements[6 * i + 1] = 0.0;
            pImpl->analogSensorData.measurements[6 * i + 2] = 0.0;
            pImpl->analogSensorData.measurements[6 * i + 3] = 0.0;
            pImpl->analogSensorData.measurements[6 * i + 4] = 0.0;
            pImpl->analogSensorData.measurements[6 * i + 5] = 0.0;
        }

    }
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
    // A single serial device will be streaming data from all the sensors from the FTShoes
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
    return detach();
}

void ftnodeDriver::threadRelease()
{}

// =============
// IAnalogSensor
// =============

int ftnodeDriver::read(yarp::sig::Vector& out)
{
    out.resize(pImpl->analogSensorData.measurements.size());

    {
        std::lock_guard<std::mutex> lock(pImpl->mutex);
        std::copy(pImpl->analogSensorData.measurements.begin(),
                  pImpl->analogSensorData.measurements.end(),
                  out.data());
    }

    return IAnalogSensor::AS_OK;
}

int ftnodeDriver::getState(int ch)
{
    // Check if channel is in the right range
    if (ch < 0 || ch > pImpl->analogSensorData.numberOfChannels) {
        yError() << LogPrefix << "Failed to get status for channel" << ch;
        yError() << LogPrefix << "Channels must be in the range 0 -"
                 << pImpl->analogSensorData.numberOfChannels;
        return IAnalogSensor::AS_ERROR;
    }

    // TODO: Check if we can handle this better according to the serial port data
    return  IAnalogSensor::AS_OK;
}

int ftnodeDriver::getChannels()
{
    std::lock_guard<std::mutex> lock(pImpl->mutex);
    return pImpl->analogSensorData.numberOfChannels;
}

int ftnodeDriver::calibrateSensor()
{
    // Not yet implemented
    return IAnalogSensor::AS_ERROR;
}

int ftnodeDriver::calibrateSensor(const yarp::sig::Vector& /*value*/)
{
    // Not yet implemented
    return IAnalogSensor::AS_ERROR;
}

int ftnodeDriver::calibrateChannel(int /*ch*/)
{
    // Not yet implemented
    return IAnalogSensor::AS_ERROR;
}

int ftnodeDriver::calibrateChannel(int /*ch*/, double /*value*/)
{
    // Not yet implemented
    return IAnalogSensor::AS_ERROR;
}
