cmake_minimum_required(VERSION 3.10)

project(NamedPipeEmployees LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server server.cpp employ.cpp)

add_executable(client client.cpp employ.cpp)

add_executable(unit unit.cpp  employ.cpp)


