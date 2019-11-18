cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(ChatClient)
    
set(SOURCE_EXE main.cpp)   

include_directories(Client)

add_executable(${PROJECT_NAME} ${SOURCE_EXE})

add_subdirectory(Client)

target_link_libraries(${PROJECT_NAME} ClientLIB)		

