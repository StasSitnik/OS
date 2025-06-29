cmake_minimum_required(VERSION 3.10)

project(EmployeeManagement)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(C:/Users/37544/Desktop/OS/lab1/V1)  

add_executable(main main.cpp)
add_executable(creator creator.cpp)
add_executable(reporter reporter.cpp)
add_executable(tests test.cpp)  
