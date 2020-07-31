/*
 * Copyright (C) 2017 iCub Facility
 * Authors: Luca Tagliapietra, Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#ifndef YARP_ftshoeDriver_H
#define YARP_ftshoeDriver_H

#include <yarp/os/Mutex.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/PreciselyTimed.h>

#include <yarp/dev/Wrapper.h>
#include <yarp/dev/PolyDriver.h>

#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>

#include <yarp/os/Property.h>

#include <stdio.h>
#include <iostream>


namespace yarp {
namespace dev {

class ftshoeDriver : public yarp::dev::IAnalogSensor,
                     public yarp::dev::DeviceDriver,
                     public yarp::dev::IPreciselyTimed,
                     public yarp::dev::IMultipleWrapper
{
private:
    // Prevent copy
    ftshoeDriver(const ftshoeDriver & other);
    ftshoeDriver & operator=(const ftshoeDriver & other);

    // Use a mutex to avoid race conditions
    yarp::os::Mutex p_mutex;

    // Buffers of sensor data and timestamp
    yarp::sig::Vector f_sensorReadings;
    yarp::sig::Vector s_sensorReadings;
    yarp::os::Stamp f_timestamp;
    yarp::os::Stamp s_timestamp;

    // Buffers for ftNode data
    bool useFTNodeDriver;
    yarp::sig::Vector ftNode_sensorReadings;
    int ftNode_firstSensorRange[2];
    int ftNode_secondSensorRange[2];

    // Buffers of output data and timestamp
    yarp::sig::Vector devout_data;
    yarp::os::Stamp devout_timestamp;

    //To open config files in this case the calib.ini file
     yarp::os::Property prop;

    // Status of the sensors
    int f_status;
    int s_status;

    int p_status;

    yarp::dev::IAnalogSensor *f_sensor_p;
    yarp::dev::IAnalogSensor *s_sensor_p;

    yarp::dev::IAnalogSensor *ftNode_sensor_p;
    yarp::os::Stamp ftNode_timestamp;
    int ftNode_status;

    // ftShoe settings
    // offset between the two ftSensors expressed in second (rear) ftSensor SoR
    yarp::sig::Vector fts_offset;
    yarp::sig::Matrix fts_orientation_R;

    yarp::sig::Matrix s_fts_to_out_R;

    // offsets compensation
    yarp::sig::Vector static_offsets;
    bool calibrated;

    // use inSitu calibration results to process fts data
    bool useInSituCalibration;
    yarp::sig::Matrix f_insitu_matrix;
    yarp::sig::Matrix s_insitu_matrix;

    void combineFtsStatus();

public:
    ftshoeDriver();
    virtual ~ftshoeDriver();

    // DeviceDriver interface
    bool open(yarp::os::Searchable &config);
    bool close();

    // IAnalogSensor interface
    virtual int read(yarp::sig::Vector &out);
    virtual int getState(int ch);
    virtual int getChannels();
    virtual int calibrateChannel(int ch, double v);
    virtual int calibrateSensor();
    virtual int calibrateSensor(const yarp::sig::Vector& value);
    virtual int calibrateChannel(int ch);

    // IPreciselyTimed interface
    virtual yarp::os::Stamp getLastInputStamp();

    // IMultipleWrapper interface
    virtual bool attachAll(const PolyDriverList &devices2Attach);
    virtual bool detachAll();

    virtual bool attach(yarp::dev::PolyDriver *poly);
    virtual bool detach();
};

}
}

#endif

