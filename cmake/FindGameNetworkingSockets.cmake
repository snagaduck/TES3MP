# Find GameNetworkingSockets (Valve, open-source non-Steam build)
# Once done this will define:
#
#  GameNetworkingSockets_FOUND   - system has GameNetworkingSockets
#  GameNetworkingSockets_INCLUDE - the include directory
#  GameNetworkingSockets_LIBRARY - link target

FIND_LIBRARY(GameNetworkingSockets_LIBRARY_RELEASE
    NAMES GameNetworkingSockets GameNetworkingSockets_s
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib64
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
    $ENV{GNS_ROOT}/lib
    $ENV{HOME}/.local/lib
)

FIND_PATH(GameNetworkingSockets_INCLUDE
    NAMES steam/steamnetworkingsockets.h
    PATHS
    ENV CPATH
    /usr/include
    /usr/include/GameNetworkingSockets
    /usr/local/include
    /usr/local/include/GameNetworkingSockets
    /opt/local/include
    $ENV{GNS_ROOT}/include
    $ENV{HOME}/.local/include/GameNetworkingSockets
)

IF(GameNetworkingSockets_INCLUDE AND GameNetworkingSockets_LIBRARY_RELEASE)
    SET(GameNetworkingSockets_FOUND TRUE)
ENDIF()

IF(GameNetworkingSockets_FOUND)
    IF(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(GameNetworkingSockets_LIBRARY
            optimized ${GameNetworkingSockets_LIBRARY_RELEASE})
    ELSE()
        SET(GameNetworkingSockets_LIBRARY ${GameNetworkingSockets_LIBRARY_RELEASE})
    ENDIF()

    IF(NOT GameNetworkingSockets_FIND_QUIETLY)
        MESSAGE(STATUS "Found GameNetworkingSockets: ${GameNetworkingSockets_LIBRARY}")
        MESSAGE(STATUS "Found GameNetworkingSockets include: ${GameNetworkingSockets_INCLUDE}")
    ENDIF()
ELSE()
    IF(GameNetworkingSockets_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find GameNetworkingSockets")
    ELSE()
        MESSAGE(STATUS "GameNetworkingSockets not found (optional for now)")
    ENDIF()
ENDIF()
