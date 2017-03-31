# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Francisco Andrade
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default
set(COMPILE_BY_DEFAULT ON)
yarp_prepare_device(ati_ethernet TYPE yarp::dev::ati_ethernetDriver
                              INCLUDE ati_ethernetDriver.h
                              WRAPPER AnalogServer)

find_package(TinyXML REQUIRED)
 include_directories( ${CMAKE_CURRENT_SOURCE_DIR})
yarp_add_plugin(ati_ethernet ati_ethernetDriver.cpp ati_ethernetDriver.h)

target_link_libraries(ati_ethernet ${YARP_LIBRARIES} ${TinyXML_LIBRARIES} )

yarp_install(TARGETS ati_ethernet
             COMPONENT runtime
             LIBRARY DESTINATION ${FORCETORQUE-YARP-DEVICES_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${FORCETORQUE-YARP-DEVICES_STATIC_PLUGINS_INSTALL_DIR})

yarp_install(FILES ati_ethernet.ini  DESTINATION ${FORCETORQUE-YARP-DEVICES_PLUGIN_MANIFESTS_INSTALL_DIR})


