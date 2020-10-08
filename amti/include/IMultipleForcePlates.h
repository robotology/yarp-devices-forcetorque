/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#ifndef IMULTIPLEFORCEPLATES_H
#define IMULTIPLEFORCEPLATES_H

#include <string>
#include <yarp/sig/Vector.h>
#include <yarp/os/Stamp.h>

namespace yarp {
namespace dev {
    class IMultipleForcePlates;
}
}

class yarp::dev::IMultipleForcePlates
{
public:

    /**
     * Virtual destructor
     */
    virtual ~IMultipleForcePlates();

    /**
     * Returns the number of force plates found in the system
     * @returns the numner of platforms
     */
    virtual int getNumberOfPlatforms() = 0;

    /**
     * Return the index corresponding to the platform identified by the specified parameter
     * @param platformID id of the platform
     * @return the index of the platform or -1 if not found
     */
    virtual int getPlatformIndexForPlatformID(const std::string& platformID) = 0;

    /**
     * Get the last measure read by the platform at the specified index
     *
     * @param[in] platformIndex index of the platform
     * @param[out] measurement vector filled with the last measurements of the force plate
     * @param[out] timestamp timestamp associated with the last measure. Pass NULL if not interested in the timestamp
     * @return the status of the measure as in yarp::dev::IAnalogSensor
     */
    virtual int getLastMeasurementForPlateAtIndex(const unsigned platformIndex,
                                                   yarp::sig::Vector& measurement,
                                                   yarp::os::Stamp *timestamp) = 0;

};

#endif // IMULTIPLEFORCEPLATES_H
