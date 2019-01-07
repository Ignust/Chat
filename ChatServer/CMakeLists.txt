cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(ChatServer)

file(GLOB CPPS "*.cpp")

add_executable(${PROJECT_NAME} ${CPPS})		
