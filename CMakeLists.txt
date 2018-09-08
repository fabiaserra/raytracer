cmake_minimum_required(VERSION 3.7)

project(RayTracer CXX)

set(CMAKE_CXX_STANDARD 11)

#file(GLOB_RECURSE LibFiles "include/*.h")
#source_group("Headers" FILES ${HEADERS})
#add_custom_target(headers SOURCES ${LibFiles})

FILE(GLOB_RECURSE HEADERS "include/*.h")
include_directories("include")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


