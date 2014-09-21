cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(VRISKA_NETWORK_LIB VriskaNetwork)
set(VRISKA_NETWORK_LIB ${VRISKA_NETWORK_LIB} PARENT_SCOPE)

set(VRISKA_NETWORK_SRCS
    src/Core/BufferRing.cpp
    src/Core/Logger.cpp
    src/Socket/INativeSocket.cpp
    src/Socket/SocketSet.cpp
    src/Socket/Socket.cpp
    src/Socket/SocketServer.cpp
    src/Socket/SocketClient.cpp
    src/Network/BlockingClient.cpp
    src/Network/Client.cpp
    src/Network/Server.cpp
    src/Network/SimpleClient.cpp)

if(MSVC)
    set(VRISKA_NETWORK_SRCS ${VRISKA_NETWORK_SRCS}
        src/Socket/WindowsSocket.cpp)
else()
    set(VRISKA_NETWORK_SRCS ${VRISKA_NETWORK_SRCS}
        src/Socket/LinuxSocket.cpp)
endif()

add_library(${VRISKA_NETWORK_LIB} ${VRISKA_LIBRARY_TYPE} ${VRISKA_NETWORK_SRCS})
set_target_properties(${VRISKA_NETWORK_LIB} PROPERTIES COMPILE_FLAGS "${VRISKA_COMPILE_FLAGS}")
target_link_libraries(${VRISKA_NETWORK_LIB} ${VRISKA_CORE_LIB} ${VRISKA_THREAD_LIB})