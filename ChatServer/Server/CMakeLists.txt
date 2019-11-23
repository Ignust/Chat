cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(Server)

set(SOURCE_LIB Server.cpp)		

add_library(ServerLIB STATIC ${SOURCE_LIB})
