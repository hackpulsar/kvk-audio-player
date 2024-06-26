cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    project(kvk_audio_player LANGUAGES CXX)
elseif(UNIX)
    project(kvk_audio_player)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra -Wno-deprecated")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -Wextra -Wno-deprecated --debug")

find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(SndFile REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.qrc"
    "${SOURCE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*/*.h"
    "${SOURCE_DIR}/*/*.cpp"
    "${SOURCE_DIR}/*/*.c"
)

#message(STATUS "SOURCES: ${SOURCES}")

file(GLOB EXTERN_INCLUDE_DIRS
     "${CMAKE_SOURCE_DIR}/extern/*/include"
)

file(GLOB EXTERN_LIB_DIRS
     "${CMAKE_SOURCE_DIR}/extern/*/lib"
)

message(STATUS "EXTERN_LIB_DIRS: ${EXTERN_LIB_DIRS}")

include_directories(${FORMS_DIR})
include_directories(${EXTERN_INCLUDE_DIRS})
include_directories(${SOURCE_DIR})

#=================== SETUP EXECTUABLE ====================#

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32) 
    add_executable(kvk_audio_player WIN32 ${SOURCES})
elseif(UNIX)
    add_executable(kvk_audio_player ${SOURCES})
endif()

# Add the target includes for MY_PROJECT 
target_include_directories(kvk_audio_player PRIVATE ${FORMS_DIR})
target_include_directories(kvk_audio_player PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_directories(kvk_audio_player PRIVATE ${EXTERN_LIB_DIRS})
target_link_libraries(kvk_audio_player Qt6::Widgets openal SndFile::sndfile fmt)
