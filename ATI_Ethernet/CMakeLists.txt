# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Francisco Andrade
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins by default
set(COMPILE_BY_DEFAULT ON)
YARP_PREPARE_PLUGIN(ati_ethernet TYPE yarp::dev::ati_ethernetDriver
                              INCLUDE ati_ethernetDriver.h
							  CATEGORY device
                              EXTRA_CONFIG WRAPPER=AnalogServer)

if(ENABLE_ati_ethernet)

	find_package(TinyXML REQUIRED)

	yarp_add_plugin(ati_ethernet ati_ethernetDriver.cpp ati_ethernetDriver.h)

	target_link_libraries(ati_ethernet ${YARP_LIBRARIES} ${TinyXML_LIBRARIES})

	yarp_install(TARGETS ati_ethernet
				COMPONENT runtime
				LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
				ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

	yarp_install(FILES ati_ethernet.ini  DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

endif()
