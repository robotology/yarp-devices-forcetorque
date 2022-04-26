# forcetorque-yarp-devices
YARP Device Drivers for various commercial Six Axis Force Torque sensors.

## Rationale
This repository contains YARP Device Drivers (see [YARP documentation on devices](http://www.yarp.it/note_devices.html) ) for exposing various commercial Six Axis Force Torque sensors in a YARP network. 
The repo contains the following YARP devices : 
* [`forcetorqueDriverExample`](forceTorqueDriverExample) : Template of a generic YARP driver for a Six Axis Force Torque sensor. 
* [`amti`](amti) : Drivers for the AMTI force plates system.

## Installation

##### Dependencies
- [YARP](https://github.com/robotology/yarp)
- For AMTI device: AMTI SDK (ask to AMTI for the libraries)

##### Step-by-step installation
* Install YARP on your platform, following the instructions on [YARP documentation](http://www.yarp.it/install.html). 
* Compile the code in this repository using [CMake](https://cmake.org/) and your preferred compiler. See [YARP documentation on how to compile a CMake project](http://www.yarp.it/using_cmake.html).
* Note that by default all devices are active. Deactivate the device you don't want to compile.
* Install the compiled devices. You can specify the installation directory using the [`CMAKE_INSTALL_PREFIX`](https://cmake.org/cmake/help/v3.0/variable/CMAKE_INSTALL_PREFIX.html) CMake option.
* Add `${CMAKE_INSTALL_PREFIX}/share/yarp` (where `${CMAKE_INSTALL_PREFIX}` needs to be substituted to the directory that you choose as the `CMAKE_INSTALL_PREFIX`) to your `YARP_DATA_DIRS` enviromental variable (for more on the `YARP_DATA_DIRS` env variable, see [YARP documentation on data directories](http://www.yarp.it/yarp_data_dirs.html) ). 
* Once you do that, you should be able to find the devices compiled by this repo (for example the `forcetorqueDriverExample`) using the command `yarp plugin forcetorqueDriverExample`, which should have an output similar to:
~~~
Yes, this is a YARP plugin
  * library:        CMAKE_INSTALL_PREFIX/lib/amti-yarp-devices/forcetorqueDriverExample.so
  * system version: 5
  * class name:     yarp::dev::forcetorqueDriverExample
  * base class:     yarp::dev::DeviceDriver
~~~
If this is not the case, there could be some problems in finding the plugin. In that case, just move yourself to the ``${CMAKE_INSTALL_PREFIX}/share/amti-yarp-devices` directory and launch the device from there.

##### Device use 
To launch the `forcetorqueDriverExample` device, you can use the example configuration file [`forcetorqueDriverExample/yarprobotinterface.xml`](forcetorqueDriverExample/yarprobotinterface.xml) for the `yarprobotinterface`.
To do so, launch the yarpserver, then on a terminal launch the device:
~~~
yarprobotinterface --from yarprobotinterface.xml
~~~
This should open a YARP port `/ft` , that you can read from the command line for example using the `yarp read` command:
~~~
yarp read ... /ft/analog:o
~~~

Maintainers
--------------
This repository is maintained by:

| | |
|:---:|:---:|
| [<img src="https://github.com/RiccardoGrieco.png" width="40">](https://github.com/RiccardoGrieco) | [@RiccardoGrieco](https://github.com/RiccardoGrieco) |

