/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francesco Romano
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "AMTIPlatformsDriver.h"

#include "AMTIlib.h"

#include <cassert>
#include <cmath>

#include <yarp/os/LockGuard.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>
#include <yarp/dev/IAnalogSensor.h>

class yarp::dev::AMTIPlatformsDriver::AMTIReaderThread : public yarp::os::RateThread
{
    yarp::dev::AMTIPlatformsDriver &driver;
    double timeout;

public:
    AMTIReaderThread(yarp::dev::AMTIPlatformsDriver& _driver, int period, double _timeout)
        : yarp::os::RateThread(period), driver(_driver), timeout(_timeout) {}

    virtual void run()
    {
        yarp::os::LockGuard guard(driver.m_mutex);

        //keeping only the last reading
        while (getCurrentData(driver.m_numOfPlatforms, driver.m_channelSize, driver.m_sensorReadings.data()))
        {
            driver.m_timestamp.update();
        }

        if (std::abs(yarp::os::Time::now() - driver.m_timestamp.getTime()) > timeout) {
            //timeout error
            driver.m_status = yarp::dev::IAnalogSensor::AS_TIMEOUT;
        }
        else if (driver.m_status != yarp::dev::IAnalogSensor::AS_ERROR) {
            //reset the status to be OK
            driver.m_status = yarp::dev::IAnalogSensor::AS_OK;
        }

    }

};


yarp::dev::AMTIPlatformsDriver::AMTIPlatformsDriver()
    : m_sensorReadings(6)
    , m_reader(0)
    , m_status(yarp::dev::IAnalogSensor::AS_ERROR)
{

    m_sensorReadings.zero();
    m_timestamp.update();

}

yarp::dev::AMTIPlatformsDriver::~AMTIPlatformsDriver()
{
}

bool yarp::dev::AMTIPlatformsDriver::open(yarp::os::Searchable &config)
{
    yarp::os::LockGuard guard(m_mutex);

    // config should be parsed for the options of the device
    int result = loadDriver();
    //if no devices have been found return false
    if (result != 2) {
        yError("Failed to initialize AMTI drivers or no devices found");
        return false;
    }

    //Now check configuration
    AMTI_CONFIGURATION_CHECK configCheck = checkDriverConfiguration();
    //TODO: decide how to handle different return value of the configuration.
    //For now only AMTI_CONFIGURATION_CHECK_SETUP_IS_EQUAL is accepted
    if (configCheck != AMTI_CONFIGURATION_CHECK_SETUP_IS_EQUAL) {
        yError("Check configuration returned %d", configCheck);
        return false;
    }

    int samplesPerSeconds = config.check("rate", yarp::os::Value(500), "number of samples per second (Hz)").asInt();
    setAcquisitionRate(samplesPerSeconds);

    double readingTimeout = config.check("timeout", yarp::os::Value(0.5), "number of seconds before timeout error (s)").asDouble();

    std::string genLock = config.check("genlock", yarp::os::Value("off"), "genlock mode (off|raise|fall)").asString();
    AMTI_GENLOCK genlockOption = AMTI_GENLOCK_OFF;

    if (genLock == "raise") {
        genlockOption = AMTI_GENLOCK_EDGE_RISING;
    }
    else if (genLock == "fall") {
        genlockOption = AMTI_GENLOCK_EDGE_FALLING;
    }
    else if (genLock != "off") {
        yError("Genlock option not recognized. Only (off|raise|fall) are allowed");
        return false;
    }
    configureGenlock(genlockOption);

    std::string dataFormatMode = config.check("dataFormat", yarp::os::Value("data"), "data format mode (data(6 channels)|ext(8 channels))").asString();
    AMTI_DATAFORMAT dataFormat = AMTI_DATAFORMAT_ONLYDATA;
    m_channelSize = 6;
    if (dataFormatMode == "ext") {
        dataFormat = AMTI_DATAFORMAT_EXTENDED;
        m_channelSize = 8;
    }
    else if (dataFormatMode != "data") {
        yError("Data format mode option not recognized. Only (data|ext) are allowed");
        return false;
    }
    configureDriverDataFormat(dataFormat);

    configureDriverRunMode(AMTI_RUNMODE_POLL);

    // Get the number of platforms and their order
    // for now this info is only printed, but it should be used to order the output data
    m_numOfPlatforms = getPlatformsCount();

    for (unsigned i = 0; i < m_numOfPlatforms; ++i) {
        char platformModelNumber[16];
        char platformSerialNumber[16];
        char platformFwVersion[16];
        char calibrationDate[12];
        getPlatformModelNumber(i, platformModelNumber);
        getPlatformSerialNumber(i, platformSerialNumber);
        getPlatformFirmwareVersion(i, platformFwVersion);
        getPlatformLastCalibrationDate(i, calibrationDate);

        yInfo("Found platform %s[%s] at index %d. Fw %s. Calibrated %s", platformModelNumber, platformSerialNumber, i,
            platformFwVersion, calibrationDate);
    }
    m_sensorReadings.resize(m_channelSize * m_numOfPlatforms);
    m_sensorReadings.zero();

    calibratePlatforms();

    //create the reader
    m_reader = new AMTIReaderThread(*this, samplesPerSeconds, readingTimeout);
    if (m_reader && m_reader->start()) {
        m_status = yarp::dev::IAnalogSensor::AS_OK;
        startAcquisition();
        return true;
    }

    return false;
}

bool yarp::dev::AMTIPlatformsDriver::close()
{
    yarp::os::LockGuard guard(m_mutex);

    m_status = yarp::dev::IAnalogSensor::AS_ERROR;
    if (m_reader) {
        m_reader->stop();
        delete m_reader;
        m_reader = 0;
    }

    stopAcquisition();
    releaseDriver();

    return true;
}

yarp::dev::AMTIPlatformsDriver::AMTIPlatformsDriver(const yarp::dev::AMTIPlatformsDriver& /*other*/)
{
    // Copy is disabled
    assert(false);
}

yarp::dev::AMTIPlatformsDriver& yarp::dev::AMTIPlatformsDriver::operator=(const yarp::dev::AMTIPlatformsDriver &other)
{
    assert(false);
    return *this;
}

int yarp::dev::AMTIPlatformsDriver::getNumberOfPlatforms()
{
    yarp::os::LockGuard guard(m_mutex);
    return m_numOfPlatforms;
}

int yarp::dev::AMTIPlatformsDriver::getPlatformIndexForPlatformID(const std::string& platformID)
{
    yarp::os::LockGuard guard(m_mutex);
    for (unsigned i = 0; i < m_numOfPlatforms; ++i) {
        char platformSerialNumber[16];
        getPlatformSerialNumber(i, platformSerialNumber);
        if (platformID == std::string(platformSerialNumber))
            return i;
    }
    return -1;

}

int yarp::dev::AMTIPlatformsDriver::getLastMeasurementForPlateAtIndex(const unsigned platformIndex,
    yarp::sig::Vector& measurement,
    yarp::os::Stamp *timestamp)
{
    yarp::os::LockGuard guard(m_mutex);
    if (platformIndex >= m_numOfPlatforms) {
        measurement.zero();
        yError("Platform index must be less than %d", m_numOfPlatforms);
        return yarp::dev::IAnalogSensor::AS_ERROR;
    }

    unsigned startingIndex = platformIndex * m_channelSize;
    if (m_channelSize == 8) startingIndex++; //skip the first element

    for (unsigned i = 0; i < 6; ++i) {
        measurement[i] = m_sensorReadings[startingIndex + i];
    }
    if (timestamp) {
        *timestamp =  m_timestamp;
    }
    return m_status;

}

yarp::os::Stamp yarp::dev::AMTIPlatformsDriver::getLastInputStamp()
{
    return m_timestamp;
}
