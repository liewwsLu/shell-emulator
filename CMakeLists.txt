cmake_minimum_required(VERSION 3.16)
project(ShellEmulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(emulator
    src/main.cpp
    src/parser.cpp
    src/system_info.cpp
    src/shell.cpp
    src/commands.cpp
)

set_target_properties(emulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin
)

if(MSVC)
    target_compile_definitions(emulator PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
