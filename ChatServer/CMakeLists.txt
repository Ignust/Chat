cmake_minimum_required(VERSION 2.8)


project(ChatServer)
    
set(SOURCE_EXE main.cpp)   
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(CWrapMail)
include_directories(DataBase)
include_directories(EventHandler)
include_directories(Listener)
include_directories(Manager)
include_directories(Server)
include_directories(Tools)
include_directories(${PROJECT_SOURCE_DIR}/../Mail/)

add_executable(${PROJECT_NAME} ${SOURCE_EXE})

add_subdirectory(DataBase)
add_subdirectory(EventHandler)
add_subdirectory(Manager)
add_subdirectory(Server)
add_subdirectory(CWrapMail)


target_link_libraries(${PROJECT_NAME} DataBaseLIB)
target_link_libraries(${PROJECT_NAME} EventHandlerLIB)
target_link_libraries(${PROJECT_NAME} ManagerLIB)
target_link_libraries(${PROJECT_NAME} ServerLIB)
target_link_libraries(${PROJECT_NAME} CWrapMailLIB)
		
		
