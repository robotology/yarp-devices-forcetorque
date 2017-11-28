/*
 * Copyright (C) 2017 iCub Facility
 * Authors: Luca Tagliapietra, Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

# Compile the plugin by default
set(COMPILE_BY_DEFAULT ON)
YARP_PREPARE_PLUGIN(ftshoe TYPE yarp::dev::ftshoeDriver
                           INCLUDE ftshoeDriver.h
			   CATEGORY device
			   EXTRA_CONFIG WRAPPER=AnalogServer)

if(ENABLE_ftshoe)

    yarp_add_plugin(ftshoe ftshoeDriver.cpp ftshoeDriver.h)

    include_directories(${YARP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ftshoe ${YARP_LIBRARIES})
    yarp_install(TARGETS ftshoe
                 COMPONENT runtime
                 LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
                 ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

    yarp_install(FILES ftshoe.ini DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

endif()
