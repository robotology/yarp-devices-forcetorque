# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default
set(COMPILE_BY_DEFAULT OFF)
YARP_PREPARE_PLUGIN(forcetorqueDriverExample TYPE yarp::dev::forcetorqueDriverExample
                                             INCLUDE forcetorqueDriverExample.h
											 CATEGORY device
                                             EXTRA_CONFIG WRAPPER=AnalogServer)

yarp_add_plugin(forcetorqueDriverExample forcetorqueDriverExample.cpp forcetorqueDriverExample.h)
include_directories(${YARP_INCLUDE_DIRS})
target_link_libraries(forcetorqueDriverExample ${YARP_LIBRARIES})  
yarp_install(TARGETS forcetorqueDriverExample
             COMPONENT runtime
             LIBRARY DESTINATION ${FORCETORQUE-YARP-DEVICES_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${FORCETORQUE-YARP-DEVICES_STATIC_PLUGINS_INSTALL_DIR})
yarp_install(FILES forcetorqueDriverExample.ini  DESTINATION ${FORCETORQUE-YARP-DEVICES_PLUGIN_MANIFESTS_INSTALL_DIR})


