/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#ifndef IMULTIPLEFORCEPLATES_H
#define IMULTIPLEFORCEPLATES_H

#include <string>

namespace yarp {
namespace dev {
    class IMultipleForcePlates;
}

namespace sig
{
    class Vector;
}

namespace os
{
    class Stamp;
}

}

class yarp::dev::IMultipleForcePlates
{
public:
    virtual ~IMultipleForcePlates();

    virtual int getNumberOfPlatforms() = 0;
    virtual int getPlatformIndexForPlatformID(const std::string& platformID) = 0;

    /**
     * @return the status of the measure as in yarp::dev::IAnalogSensor
     */
    virtual int getLastMeasurementForPlateAtIndex(const unsigned platformIndex,
                                                   yarp::sig::Vector& measurement,
                                                   yarp::os::Stamp *timestamp) = 0;

};

#endif // IMULTIPLEFORCEPLATES_H
