cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(Client)

include_directories(${PROJECT_SOURCE_DIR}/../../Mail/)

set(SOURCE_LIB Client.cpp)		

add_library(ClientLIB STATIC ${SOURCE_LIB})
