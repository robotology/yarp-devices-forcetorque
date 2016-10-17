# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default
set(COMPILE_BY_DEFAULT ON)
yarp_prepare_device(optoforce TYPE yarp::dev::optoforceDriver
                              INCLUDE optoforceDriver.h
                              WRAPPER AnalogServer)

yarp_add_plugin(optoforce optoforceDriver.cpp optoforceDriver.h)


include_directories(${YARP_INCLUDE_DIRS} /home/francisco/apps/LINUX_API_v1.4.5/include/ )
target_link_libraries(optoforce /home/francisco/apps/LINUX_API_v1.4.5/lib/libOMD.so.1.4.5)
yarp_install(TARGETS optoforce
             COMPONENT runtime
             LIBRARY DESTINATION ${FORCETORQUE-YARP-DEVICES_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${FORCETORQUE-YARP-DEVICES_STATIC_PLUGINS_INSTALL_DIR})

yarp_install(FILES optoforce.ini  DESTINATION ${FORCETORQUE-YARP-DEVICES_PLUGIN_MANIFESTS_INSTALL_DIR})


