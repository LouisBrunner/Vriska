cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Vriska CXX)

#Conf
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(WARNING "In-source builds are not encouraged. Make a separate folder for building:\nmkdir build; cd build; cmake ..; cd ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif()

if(VRISKA_LIBRARY_TYPE)
	if ((NOT VRISKA_LIBRARY_TYPE STREQUAL "SHARED")
		AND (NOT VRISKA_LIBRARY_TYPE STREQUAL "STATIC"))
		message(FATAL_ERROR "Unknown type of build: ${VRISKA_LIBRARY_TYPE} [SHARED|STATIC]")
	endif()
else()
	set(VRISKA_LIBRARY_TYPE SHARED)
endif()
#/Conf

#Common vars
set(BUILD_FOLDER ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH "${BUILD_FOLDER}/bin")
set(LIBRARY_OUTPUT_PATH "${BUILD_FOLDER}/lib")

set(VRISKA_COMPILE_FLAGS "${CMAKE_CXX_FLAGS}")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(VRISKA_COMPILE_FLAGS "${VRISKA_COMPILE_FLAGS} -fvisibility=hidden")
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        set(VRISKA_COMPILE_FLAGS "${VRISKA_COMPILE_FLAGS} -Wall -Wextra -Wshadow -Wundef")
    elseif(MSVC)
        set(VRISKA_COMPILE_FLAGS "${VRISKA_COMPILE_FLAGS} /wd4251")
    endif()
endif()

if(APPLE)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
endif()
#/Common vars

add_definitions(-DVRISKA_LIB_EXPORT)

set(VRISKA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(VRISKA_LIBRARIES_DIRS ${PROJECT_BINARY_DIR}/lib)

include_directories(${VRISKA_INCLUDE_DIR})

add_subdirectory(Core)
add_subdirectory(Threads)
add_subdirectory(Network)

set(VRISKA_LIBRARIES
    ${VRISKA_CORE_LIB}
    ${VRISKA_THREAD_LIB}
    ${VRISKA_NETWORK_LIB})

if(VRISKA_BUILD_TESTS)
    add_subdirectory(tests)
endif()
