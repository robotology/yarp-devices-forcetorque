# How to install the ftShoes

The following steps are required to install the ftShoes on a brand new Windows setup.

### Prerequisites

- CMake
- Git
- Rapid Environment Editor
- Visual Studio 2017
- USB-CAN driver
- Root folder where repos/drivers will be installed

  ```bash
  mkdir dev
  ```
  
### Install dependencies via vcpkg
See link [here](https://github.com/robotology/robotology-superbuild#vcpkg).  The official guidelines are [here](https://github.com/Microsoft/vcpkg).

```bash
cd dev
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
```
From a `cmd` terminal

```bash
.\vcpkg.exe integrate install
.\vcpkg integrate install
```

to have, respectively

- the user-wide integration for the vcpkg root
- the tab/auto-completion in PowerShell

Install vcpkg

```bash
.\vcpkg install --triplet x64-windows ace gsl eigen3 freeglut ode sdl2 qt5 libxml2
```
**Set the environment variables**

on Rapid Environment (as expandable string) as follows

|   Variable              |  Setting                                                 | 
|:-----------------------:|:--------------------------------------------------------:|
|  `CMAKE_PROGRAM_PATH`   | `C:{your-path}\dev\vcpkg\installed\x64-windows\tools`    |
|  `CMAKE_PREFIX_PATH`    | `C:{your-path}\dev\vcpkg\installed\x64-windows`          |
|                         | `C:{your-path}\dev\vcpkg\installed\x64-windows\debug`    |
|   `PATH`                | `C:{your-path}\dev\vcpkg\installed\x64-windows\debug\bin`|
|                         | `C:{your-path}\dev\vcpkg\installed\x64-windows\bin`      |

### icub-firmware-shared
Refer to [icub-firmware-shared](https://github.com/robotology/icub-firmware-shared) repo.
 
``` bash 
cd dev
git clone https://github.com/robotology/icub-firmware-shared.git
cd icub-firmware-shared
mkdir build
cd build
```

**Configure and generate**

via CMake GUI (or otherwise via terminal)
- SOURCE CODE: *icub-firmware-shared*
- BUILD IN: *icub-firmware-shared/build*
- Configure
- Generate

### Install robotology-superbuild
Refer to [robotology-superbuild](https://github.com/robotology/robotology-superbuild) repo for Windows installation.

```bash
cd dev
git clone https://github.com/robotology/robotology-superbuild.git
cd robotology-superbuild
mkdir build
cd build
```

**Set the environment variables**

on Rapid Environment (as expandable string) as follows

|   Variable              |  Setting                                                 | 
|:-----------------------:|:--------------------------------------------------------:|
|  `ROBOTOLOGY_SUPERBUILD_ROOT`   | (as string) pointing to the directory where the robotology-superbuild was cloned    |
|   `PATH`                | `C:{your-path}\dev\robotology-superbuild\build\install\bin`|
|                         | `C:{your-path}\dev\robotology-superbuild\build\install\lib`|
|   `YARP_DATA_DIRS`      | `C:{your-path}\dev\robotology-superbuild\build\install\share\yarp`|
|                         | `C:{your-path}\dev\robotology-superbuild\build\install\share\iCub`|
|                         | `C:{your-path}\dev\robotology-superbuild\build\install\share\ICUBcontrib`|
|                         | `C:{your-path}\dev\robotology-superbuild\robotology\icub-tests\suits`|

 

**Configure and generate**

- SOURCE CODE: *robotology-superbuild*
- BUILD IN: *robotology-superbuild/build*
- Flag on `ROBOTOLOGY_ENABLE_DYNAMICS`
- `CMAKE_INSTALL_PREFIX = robotology-superbuild/build/install`
- Configure
- Generate

**Compile**

```bash
cmake --build . --config Release
```

### Modify icub-main options

- SOURCE CODE: *robotology-superbuild/robotology/ICUB*
- BUILD IN: *robotology-superbuild/build/robotology/ICUB*
- Flag on `ENABLE_icubmod_ecan` `ENABLE_icubmod_canBusAnalogSensor`
`ENABLE_icubmod_embObjFTsensor`
`ICUB_USE_icub_firmware_shared` `ENABLE_icubmod_gazecontrollerclient` `ENABLE_icubmod_cartesiancontrollerclient`
`ENABLE_icubmod_cartesiancontrollerserver`
- Append to `icub_firmware_shared_DIR` the folder path to the `build` dir of `icub_firmware_shared`
- Configure
- Generate
 
**Compile and install**

```bash
cd robotology-superbuild/build/robotology/ICUB
cmake --build . --config Release
cmake --build . --config Release --target INSTALL
```

### Install YARP drivers for ftShoes
Refer to [forcetorque-yarp-devices](https://github.com/robotology-playground/forcetorque-yarp-devices) repo.

``` bash 
cd dev
git clone https://github.com/robotology-playground/forcetorque-yarp-devices.git
cd forcetorque-yarp-devices
mkdir build
cd build
```

**Configure and generate**

- SOURCE CODE: *forcetorque-yarp-devices*
- BUILD IN: *forcetorque-yarp-devices/build*
- Set `CMAKE_INSTALL_PREFIX` `robotology-superbuild/build/install`
- Configure
- Generate

**Compile and install**

```bash
cmake --build . --config Release --target INSTALL
```

# How to use the ftShoes

* Launch `yarp server`
* Connect the USB-CAN interface on the laptop
* Run the driver for the PRO.0X ftShoes

  ```bash
  cd your-driver-folder/forcetorque-yarp-devices/ftShoe/conf
  yarprobotinterface.exe --config ftshoes_yarprobotinterface_PRO.0X.xml
  ```
  and wait for the message `yarprobotinterface running happily`.

* Calibrate the offset for both shoes.  Each shoe has to be positioned on a rigid support in the middle of the two sensors.
 
  - Run for the right shoe the following:
    ```bash
    yarp rpc /ft/ftShoe_Right/analog:o/rpc:i
    iana cal
    ```
    
      and check `yarp read ... /ft/ftShoe_Right/analog:o`

  - Run for the left shoe the following:
    ```bash
    yarp rpc /ft/ftShoe_Left/analog:o/rpc:i
    iana cal
    ```
    
      and check
    `yarp read ... /ft/ftShoe_Left/analog:o`


* If the driver is `running happily`, try to stream data from the shoes 
  (e.g.,`yarp read ... /ft/ftShoe/Left/analog:o`).

