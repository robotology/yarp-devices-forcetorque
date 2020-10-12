/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#ifndef YARP_AMTIFORCEPLATE_H
#define YARP_AMTIFORCEPLATE_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/PreciselyTimed.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/Wrapper.h>
#include <string>

#include <iDynTree/Core/Transform.h>
#include <iDynTree/Core/Rotation.h>

namespace yarp {
    namespace dev {
        class AMTIForcePlate;
        class IMultipleForcePlates;
    }
}


class yarp::dev::AMTIForcePlate :
    public yarp::dev::DeviceDriver,
    public yarp::dev::IPreciselyTimed,
    public yarp::dev::IAnalogSensor,
    public yarp::dev::IWrapper,
    public yarp::dev::IMultipleWrapper
{
    // Prevent copy 
    AMTIForcePlate(const AMTIForcePlate &other);
    AMTIForcePlate& operator=(const AMTIForcePlate &other);

    // Use a mutex to avoid race conditions
    yarp::os::Mutex m_mutex;

    // Platform rotation
    float m_rotation_angle; //Rotation about z axis
    iDynTree::Transform m_transform;

    // Buffers of sensor data and timestamp
    yarp::sig::Vector m_sensorReadings;
    yarp::os::Stamp m_timestamp;
    
    int m_status; /*!< status of the driver */
    IMultipleForcePlates *m_platformDriver; /*!< Pointer to the attached driver */
    unsigned m_platformIndex; /*!< Index of the considered platform */
    std::string m_platformID; /*!< Identifier of the considered platform */

public:

    AMTIForcePlate();
    virtual ~AMTIForcePlate();

    // DeviceDriver interface 
    bool open(yarp::os::Searchable &config);
    bool close();

    // IAnalogSensor interface
    virtual int read(yarp::sig::Vector &out);
    virtual int getState(int ch);
    virtual int getChannels();
    virtual int calibrateSensor();
    virtual int calibrateSensor(const yarp::sig::Vector &value);
    virtual int calibrateChannel(int ch);
    virtual int calibrateChannel(int ch, double value);

    // IPreciselyTimed interface
    virtual yarp::os::Stamp getLastInputStamp();

    // IWrapper interface
    virtual bool attach(yarp::dev::PolyDriver *poly);
    virtual bool detach();

    // IMultipleWrapper interface
    virtual bool attachAll(const yarp::dev::PolyDriverList &);
    virtual bool detachAll();

};


#endif //YARP_AMTIFORCEPLATE_H
