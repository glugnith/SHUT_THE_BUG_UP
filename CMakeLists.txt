cmake_minimum_required (VERSION 2.8.9)
project (srb-decompressor CXX C)

set (srb-decompressor_VERSION_MAJOR 0)
set (srb-decompressor_VERSION_MINOR 7)
set (srb-decompressor_VERSION_PATCH 0)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Detect OS
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message (STATUS "Operating system detected: Linux")
  set (LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  message (STATUS "Operating system detected: OSX")
  set (OSX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message (STATUS "Operating system detected: Windows")
  set (WINDOWS TRUE)
else ()
  message (ERROR "Operating system not recognised. Expected Windows, Linux or Darwin, but found ${CMAKE_SYSTEM_NAME}")
endif ()


# Include directories
include_directories (inc)
include_directories (lib)

if (LINUX AND EXISTS "${PROJECT_SOURCE_DIR}/linux_dependencies")
  include_directories (linux_dependencies)
  set (EXT_LIB_PATH "${PROJECT_SOURCE_DIR}/linux_dependencies" CACHE STRING "Absolute path to libraries")
elseif (OSX AND EXISTS "${PROJECT_SOURCE_DIR}/osx_dependencies")
  include_directories (osx_dependencies)
  set (EXT_LIB_PATH "${PROJECT_SOURCE_DIR}/osx_dependencies" CACHE STRING "Absolute path to libraries")
elseif (WINDOWS AND EXISTS "${PROJECT_SOURCE_DIR}/windows_dependencies")
  set (EXT_LIB_PATH "${PROJECT_SOURCE_DIR}/lib/windows_specific" CACHE STRING "Absolute path to libraries")
else ()
  set (EXT_LIB_PATH "/usr/local/bin" CACHE STRING "Absolute path to libraries")
  if (OSX)
    message(ERROR "Please run first install_dependencies.sh osx")
  elseif (WINDOWS)
    message(ERROR "Please run first install_dependencies.bat")
  endif()
endif ()

# Compiler and linkers flags
if (LINUX)
  # Static Build Extra Flags for Linux Static Build
  set (BUILD_SHARED_LIBS OFF)
  set (CMAKE_EXE_LINKER_FLAGS "-static")
  set (CMAKE_FIND_LIBRARY_SUFFIXES .a)
  set (CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
  set (CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
  set (CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
  set (CMAKE_SHARED_LIBRARY_CXX_FLAGS)
  set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
  set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
  set (Boost_USE_STATIC_LIBS ON)             # only find static libs
elseif (OSX)
  # Mac build not fully static. include system libraries to be dynamic
  set (CMAKE_EXE_LINKER_FLAGS)
  set (Boost_USE_STATIC_LIBS ON)             # only find static libs
elseif (WINDOWS)
  set (Boost_USE_STATIC_LIBS ON)             # only find static libs
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") # also requires boost with runtime-link=static
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") # also requires boost with runtime-link=static
endif ()

# Build type
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE  "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
find_package (Threads REQUIRED)

# Flags specific to Unix
if (LINUX OR OSX)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unknown-pragmas") # no unknows pragmas warnings
  set (CMAKE_C_FLAGS " -DSQLITE_ENABLE_UNLOCK_NOTIFY -DSQLITE_OMIT_LOAD_EXTENSION") # Needed to statically link ODB
  find_package (Boost COMPONENTS filesystem iostreams program_options log thread system REQUIRED )
  include_directories (${Boost_INCLUDE_DIR} )
endif ()


if (LINUX OR OSX)
  set (THIRD_PARTY_LIBRARIES
      ${Boost_LIBRARIES}
      ${EXT_LIB_PATH}/libbz2.a
      ${EXT_LIB_PATH}/libz.a
      ${CMAKE_THREAD_LIBS_INIT}
     )
elseif (WINDOWS)
  set (THIRD_PARTY_LIBRARIES
      ${CMAKE_THREAD_LIBS_INIT}
  )
endif ()


set (MOD_SRB_SOURCES
    src/srb/decompressor.cpp
    )
add_library(mod_srb STATIC ${MOD_SRB_SOURCES})
target_link_libraries(mod_srb ${THIRD_PARTY_LIBRARIES})



# Libraries to link with executables
if (LINUX OR OSX)
  set (LIBRARIES_TO_LINK
      mod_srb
      ${Boost_LIBRARIES}
      ${EXT_LIB_PATH}/libbz2.a
      ${EXT_LIB_PATH}/libz.a
      ${CMAKE_THREAD_LIBS_INIT}
     )
elseif (WINDOWS)
  set (LIBRARIES_TO_LINK
      mod_srb
      ${CMAKE_THREAD_LIBS_INIT}
     )
endif ()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# building binaries
add_executable(srb_decompressor src/main.cpp)
target_link_libraries(srb_decompressor ${LIBRARIES_TO_LINK})
