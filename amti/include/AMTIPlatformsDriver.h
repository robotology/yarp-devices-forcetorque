/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francesco Romano
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#ifndef YARP_AMTIPLATFORMSDRIVER_H
#define YARP_AMTIPLATFORMSDRIVER_H

#include <yarp/os/Mutex.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/PreciselyTimed.h>
#include <yarp/os/PeriodicThread.h>
#include "IMultipleForcePlates.h"

#include <yarp/sig/Vector.h>

namespace yarp {
    namespace dev {
        class AMTIPlatformsDriver;
    }
}

class yarp::dev::AMTIPlatformsDriver : public yarp::dev::IMultipleForcePlates,
                                       public yarp::dev::DeviceDriver,
                                       public yarp::os::PeriodicThread,
                                       public yarp::dev::IPreciselyTimed
{
private:

    // Use a mutex to avoid race conditions
    yarp::os::Mutex m_mutex;

    // Buffers of sensor data and timestamp
    yarp::sig::Vector m_sensorReadings;
    yarp::os::Stamp m_timestamp;

    // Properties of the sensor
    unsigned m_numOfPlatforms; /*!< Number of platforms connected to the system */
    unsigned m_channelSize; /*!< Size of the data read from the platform */

    double m_periodInSeconds;
    double m_readingTimeout;

    bool m_firstData;

    // PeriodThread
    void run() override;
    void threadRelease() override;

    int m_status; /*!< status of the driver */

public:
    AMTIPlatformsDriver();
    virtual ~AMTIPlatformsDriver();

    // DeviceDriver interface
    bool open(yarp::os::Searchable &config);
    bool close();

    //IMultipleForcePlates interface
    virtual int getNumberOfPlatforms();
    virtual int getPlatformIndexForPlatformID(const std::string& platformID);
    virtual int getLastMeasurementForPlateAtIndex(const unsigned platformIndex,
        yarp::sig::Vector& measurement,
        yarp::os::Stamp *timestamp);

    // IPreciselyTimed interface
    virtual yarp::os::Stamp getLastInputStamp();

};

#endif //end of YARP_AMTIPLATFORMSDRIVER_H
