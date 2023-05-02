cmake_minimum_required(VERSION 3.22)

project(REDIS_CLONE)
add_executable(${PROJECT_NAME} server.cpp)
add_library(hashtable hashtable.h hashtable.cpp)
target_link_libraries(${PROJECT_NAME} hashtable)