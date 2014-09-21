cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src src/*.cpp)
foreach(TEST_SOURCE ${TEST_SOURCES})
    string(REPLACE ".cpp" "" TEST_NAME ${TEST_SOURCE})
    add_executable(${TEST_NAME} src/${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} ${VRISKA_LIBRARIES})
endforeach()