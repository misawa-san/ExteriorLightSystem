cmake_minimum_required(VERSION 3.5)
project(targetapp C)

add_library(libapp STATIC
./FrontSpotLights.c
)
target_include_directories(libapp PUBLIC
../stub
)
target_compile_options(libapp PRIVATE -save-temps=obj -coverage -g -fsanitize=address -fno-omit-frame-pointer)

# Linker Flags
set(ld_flags "-fsanitize=address -coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags}")

target_compile_definitions(libapp PRIVATE
)

set_target_properties(libapp
    PROPERTIES
    LINKER_LANGUAGE C
)

