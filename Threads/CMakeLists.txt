cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_definitions(-DVRISKA_LIB_EXPORT)

set(VRISKA_THREAD_LIB VriskaThreads)
set(VRISKA_THREAD_LIB ${VRISKA_THREAD_LIB} PARENT_SCOPE)

set(VRISKA_THREAD_SRCS
    src/CondVar/CondVar.cpp
    src/CondVar/INativeCondVar.cpp
    src/Mutex/INativeMutex.cpp
    src/Mutex/Mutex.cpp
    src/Thread/INativeThread.cpp
    src/Thread/Thread.cpp
    src/Core/ScopedLock.cpp
    src/Core/Timer.cpp
    src/Core/ThreadPool.cpp)

if(MSVC)
    set(VRISKA_THREAD_SRCS ${VRISKA_THREAD_SRCS}
        src/CondVar/WindowsCondVar.cpp
        src/Mutex/WindowsMutex.cpp
        src/Thread/WindowsThread.cpp
        src/Startup.cpp)
else()
    set(VRISKA_THREAD_SRCS ${VRISKA_THREAD_SRCS}
        src/CondVar/LinuxCondVar.cpp
        src/Mutex/LinuxMutex.cpp
        src/Thread/LinuxThread.cpp)
endif()

set(VRISKA_THREAD_DEPS)
if(UNIX)
    set(VRISKA_THREAD_DEPS
        pthread)
    if(NOT APPLE)
        set(VRISKA_THREAD_DEPS ${VRISKA_THREAD_DEPS}
            rt)
    endif()
endif()

add_library(${VRISKA_THREAD_LIB} ${VRISKA_LIBRARY_TYPE} ${VRISKA_THREAD_SRCS})
set_target_properties(${VRISKA_THREAD_LIB} PROPERTIES COMPILE_FLAGS "${VRISKA_COMPILE_FLAGS}")
target_link_libraries(${VRISKA_THREAD_LIB} ${VRISKA_CORE_LIB} ${VRISKA_THREAD_DEPS})
