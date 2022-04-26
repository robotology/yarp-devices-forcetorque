# AMTI

## Dependencies

* [YARP](https://github.com/robotology/yarp)
* AMTI drivers (Windows only).

## Device architecture

The current YARP driver implementation is composed of two layers.
The first layer (`amtiplatforms`) directly interfaces with the AMTI drivers.
It opens a YARP device implementing the (custom) interface `IMultipleForcePlates`.
This device exposes all the force platforms connected to the system.

The second layer (`amtiforceplate`) interact the the first layer and exposes an `AnalogSensor` interface. 
One instance of `amtiforceplate` must be created for each force plate, and properly configured with the platform amplifier identifier.

For example, if two plates are present in the system, one and only one `amtiplatforms` has to be created, while two `amtiforceplate` must be created and attached to the `amtiplatforms` device.

See the example of `yarprobotinterface_amti.xml` file


## Setup the environment

In order to use the force platforms, the following steps must be performed before building the YARP device:

1. Create the environment variable `AMTIDriver_ROOT` pointing to the root of the SDK folder.  
  This will make the SDK visible to the build process.
2. Add the SDK folder the `PATH`.
