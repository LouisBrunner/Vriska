cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(VRISKA_CORE_LIB VriskaCore)
set(VRISKA_CORE_LIB ${VRISKA_CORE_LIB} PARENT_SCOPE)

set(VRISKA_LIBRARIES ${VRISKA_LIBRARIES}
	${VRISKA_CORE_LIB}
	PARENT_SCOPE)

set(VRISKA_CORE_SRCS
    src/Error.cpp
    src/Exceptions.cpp
    src/OS.cpp
    src/StringUtils.cpp
    src/Time.cpp
    src/Utils.cpp)

add_library(${VRISKA_CORE_LIB} ${VRISKA_LIBRARY_TYPE} ${VRISKA_CORE_SRCS})
set_target_properties(${VRISKA_CORE_LIB} PROPERTIES COMPILE_FLAGS "${VRISKA_COMPILE_FLAGS}")