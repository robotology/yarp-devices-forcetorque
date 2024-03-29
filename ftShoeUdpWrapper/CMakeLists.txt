# Copyright (C) 2018 iCub Facility
# Authors: Diego Ferigo, Luca Tagliapietray
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Do not compile the plugin by default
yarp_prepare_plugin(ftShoeUdpWrapper TYPE yarp::dev::ftShoeUdpWrapper
                                     INCLUDE ftShoeUdpWrapper.h
                                     DEFAULT OFF
                                     CATEGORY device)

if(ENABLE_ftShoeUdpWrapper)

    find_package(Asio REQUIRED)

    yarp_add_plugin(ftShoeUdpWrapper ftShoeUdpWrapper.cpp ftShoeUdpWrapper.h)

    target_include_directories(ftShoeUdpWrapper PUBLIC
        ${Asio_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ftShoeUdpWrapper ${YARP_LIBRARIES})

    yarp_install(TARGETS ftShoeUdpWrapper
                 COMPONENT runtime
                 LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
                 ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

    yarp_install(FILES ftShoeUdpWrapper.ini DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

endif()
