cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(CWrapMail)

include_directories(${PROJECT_SOURCE_DIR}/../../Mail/)

set(SOURCE_LIB CWrapMail.cpp)		

add_library(CWrapMailLIB STATIC ${SOURCE_LIB})
