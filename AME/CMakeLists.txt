# Copyright: (C) 2017 Fondazione Istituto Italiano di Tecnologia
# Authors: Francisco Andrade
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default

if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
          "Choose the type of build, options are: Debug Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_C_FLAGS_DEBUG "-Og -mssse3 -msse4.1 -msse4.2 -g -pipe -Wall -Wno-unused-variable  -Werror -Wpedantic")
set(CMAKE_C_FLAGS_RELEASE "-O3 -mssse3 -msse4.1 -msse4.2 -pipe -Wall -Wno-unused-variable -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -Og -mssse3 -msse4.1 -msse4.2 -g -pipe -Wall -Wno-unused-variable -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O3 -mssse3 -msse4.1 -msse4.2 -pipe -Wall -Wno-unused-variable -Wno-maybe-uninitialized -Werror -Wpedantic")


set(COMPILE_BY_DEFAULT ON)

YARP_PREPARE_PLUGIN(ame TYPE yarp::dev::amedriver
                              INCLUDE amedriver.h
                                                          CATEGORY device
                              EXTRA_CONFIG WRAPPER=AnalogServer)
if(ENABLE_ame)
find_package(TinyXML REQUIRED)
 include_directories( ${CMAKE_CURRENT_SOURCE_DIR})
yarp_add_plugin(ame amedriver.cpp amedriver.h)
    include_directories(${YARP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

#Library Target
add_library(AMElib STATIC
  ftSensorNew.cpp
  baseTypeEncoding.cpp
  MultitorqueEncoder.cpp
  osutil.cpp ethService.cpp
  )
set_target_properties(AMElib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(ame ${YARP_LIBRARIES} ${TinyXML_LIBRARIES} AMElib)

yarp_install(TARGETS ame
             COMPONENT runtime
             LIBRARY DESTINATION ${FORCETORQUE-YARP-DEVICES_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${FORCETORQUE-YARP-DEVICES_STATIC_PLUGINS_INSTALL_DIR})

yarp_install(FILES ame.ini  DESTINATION ${FORCETORQUE-YARP-DEVICES_PLUGIN_MANIFESTS_INSTALL_DIR})
endif()




