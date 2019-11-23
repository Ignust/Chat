cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(Manager)

include_directories(${PROJECT_SOURCE_DIR}/../Listener/)
include_directories(${PROJECT_SOURCE_DIR}/../DataBase/)
include_directories(${PROJECT_SOURCE_DIR}/../Tools/)
include_directories(${PROJECT_SOURCE_DIR}/../CWrapMail/)
include_directories(${PROJECT_SOURCE_DIR}/../../Mail/)

set(SOURCE_LIB Manager.cpp)		

add_library(ManagerLIB STATIC ${SOURCE_LIB})
