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



