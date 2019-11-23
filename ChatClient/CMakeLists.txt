cmake_minimum_required(VERSION 2.8)

project(ChatClient)
    
set(SOURCE_EXE main.cpp)   
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(Client)

add_executable(${PROJECT_NAME} ${SOURCE_EXE})

add_subdirectory(Client)

target_link_libraries(${PROJECT_NAME} ClientLIB)		

