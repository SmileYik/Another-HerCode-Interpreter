cmake_minimum_required(VERSION 3.5)

project(hercode-vm VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_VM_SOURCES
    src/console.cpp
    src/hercode_vm.h
    src/hercode_vm.cpp
)

add_executable(hercode-console ${PROJECT_VM_SOURCES})
