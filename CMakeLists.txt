cmake_minimum_required(VERSION 3.0)

project(SpanningTree)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Set toolchain flags
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. \
                         Please use a different C++ compiler.")
endif()

#if(CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
#endif()

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDE})
file(GLOB_RECURSE PROJECT_INCLUDE "${INCLUDE}/*.hpp")

set(SOURCE ${CMAKE_SOURCE_DIR}/source)

file(GLOB SPANNING_TREE_SOURCE
    ${SOURCE}/port_protocol_migration_sm.cpp
    ${SOURCE}/port_receive_sm.cpp
    ${SOURCE}/port_timers_sm.cpp
#${SOURCE}/port_protocol_migration_sm.cpp
    ${SOURCE}/bpdu.cpp
    ${SOURCE}/bridge.cpp
    ${SOURCE}/bridge_id.cpp
    ${SOURCE}/mac.cpp
#${SOURCE}/management.cpp
    ${SOURCE}/path_cost.cpp
    ${SOURCE}/port.cpp
    ${SOURCE}/port_id.cpp
    ${SOURCE}/priority_vector.cpp
    ${SOURCE}/sm_procedures.cpp
#    ${SOURCE}/state_machine.cpp
    ${SOURCE}/time.cpp
    )

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_INCLUDE} ${SPANNING_TREE_SOURCE} ${SOURCE}/main.cpp)

add_subdirectory(test)