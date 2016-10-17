# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default
set(COMPILE_BY_DEFAULT ON)
yarp_prepare_device(optoforce TYPE yarp::dev::forcetorqueDriverExample
                              INCLUDE optoforceDriver.h
                              WRAPPER AnalogServer)

yarp_add_plugin(optoforce optoforceDriver.cpp optoforceDriver.h)

include_directories(${YARP_INCLUDE_DIRS})
target_link_libraries(optoforce ${YARP_LIBRARIES})  
yarp_install(TARGETS optoforce
             COMPONENT runtime
             LIBRARY DESTINATION ${AMTI_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${AMTI_STATIC_PLUGINS_INSTALL_DIR})
             
yarp_install(FILES optoforce.ini  DESTINATION ${AMTI_PLUGIN_MANIFESTS_INSTALL_DIR})


