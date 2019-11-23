cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(EventHandler)

include_directories(${PROJECT_SOURCE_DIR}/../Manager/)
include_directories(${PROJECT_SOURCE_DIR}/../Server/)
include_directories(${PROJECT_SOURCE_DIR}/../Listener/)

set(SOURCE_LIB EventHandler.cpp)		

add_library(EventHandlerLIB STATIC ${SOURCE_LIB})
