/*
 * Copyright (C) 2019 iCub Facility
 * Authors: Yeshasvi Tirupachuri
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#ifndef YARP_ftnodeDriver_H
#define YARP_ftnodeDriver_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/Wrapper.h>
#include <yarp/dev/SerialInterfaces.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/dev/PreciselyTimed.h>
#include <yarp/os/PeriodicThread.h>


namespace yarp {
    namespace dev {
        class ftnodeDriver;
    } // namespace dev
} // namespace yarp

class yarp::dev::ftnodeDriver :
        //public yarp::dev::ISerialDevice,
        public yarp::dev::IAnalogSensor,
        public yarp::dev::DeviceDriver,
        //public yarp::dev::IPreciselyTimed,
        public yarp::os::PeriodicThread,
        public yarp::dev::IWrapper,
        public yarp::dev::IMultipleWrapper
{

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;

public:
      ftnodeDriver();
      ~ftnodeDriver() override;

      // DeviceDriver
      bool open(yarp::os::Searchable& config) override;
      bool close() override;

      // IPreciselyTimed
      // TODO: Check if we need this interface
      //yarp::os::Stamp getLastInputStamp() override;

      // PeriodicThread
      void run() override;
      void threadRelease() override;

      // IWrapper interface
      bool attach(yarp::dev::PolyDriver* poly) override;
      bool detach() override;

      // IMultipleWrapper interface
      bool attachAll(const yarp::dev::PolyDriverList& driverList) override;
      bool detachAll() override;

      // IAnalogSensor interface
      int read(yarp::sig::Vector& out) override;
      int getState(int ch) override;
      int getChannels() override;
      int calibrateSensor() override;
      int calibrateSensor(const yarp::sig::Vector& value) override;
      int calibrateChannel(int ch) override;
      int calibrateChannel(int ch, double value) override;
};

#endif // YARP_ftnodeDriver_H
