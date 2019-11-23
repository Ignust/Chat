cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(DataBase)

set(SOURCE_LIB DataBase.cpp)		

add_library(DataBaseLIB STATIC ${SOURCE_LIB})
