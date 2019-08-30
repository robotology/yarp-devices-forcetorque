#/*
# * Copyright (C) 2019 iCub Facility
# * Authors: Yeshasvi Tirupachuri
# * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
# */

# Compile the plugin by default
YARP_PREPARE_PLUGIN(ftnode TYPE yarp::dev::ftnodeDriver
                           INCLUDE ftnodeDriver.h
                           DEFAULT ON
                           CATEGORY device)

if (ENABLE_ftnode)

    yarp_add_plugin(ftnode ftnodeDriver.cpp ftnodeDriver.h)

    include_directories(${YARP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ftnode ${YARP_LIBRARIES})
    yarp_install(TARGETS ftnode
                 COMPONENT runtime
                 LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
                 ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

    yarp_install(FILES ftnode.ini DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

endif()
