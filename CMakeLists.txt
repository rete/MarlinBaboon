#################################################
# cmake file for building MarlinBaboon processors
# @author Eté Rémi ,IPNL
# Copyright (c) CNRS / IPNL
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
#################################################

# project name
PROJECT( MarlinBaboon )


SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 5 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )


# --------------- ilcsoft dependencies -----------
LIST( APPEND CMAKE_MODULE_PATH $ILCSOFT )
LIST( APPEND CMAKE_MODULE_PATH cmake/Modules )

# load default settings from ILCSOFT_CMAKE_MODULES
FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES REQUIRED )
INCLUDE( ilcsoft_default_settings )

# -------------- packages -------------
FIND_PACKAGE( Baboon REQUIRED )
FIND_PACKAGE( LCIO REQUIRED )
FIND_PACKAGE( Marlin REQUIRED )

FOREACH( pkg Baboon LCIO Marlin )
    IF( ${pkg}_FOUND )
        INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
        LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
        MESSAGE( "${pkg}_LIBRARIES : ${${pkg}_LIBRARIES}" )
        ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )
        MESSAGE ( "${pkg}_DEFINITIONS : ${${pkg}_DEFINITIONS}" )
    ENDIF()
ENDFOREACH()



OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" OFF )

IF( INSTALL_DOC )

    FIND_PACKAGE( Doxygen )

    IF( DOXYGEN_FOUND )
        ADD_SUBDIRECTORY( ./doc )
    ELSE()
        MESSAGE( SEND_ERROR "Could not find doxygen required to build documentation" )
        MESSAGE( "Please install doxygen or set INSTALL_DOC to OFF" )
        MESSAGE( "" )
    ENDIF()

ENDIF()


ADD_SUBDIRECTORY( ./source )


# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

