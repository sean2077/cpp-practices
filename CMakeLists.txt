cmake_minimum_required(VERSION 3.10)

project(tmp)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/util)

# 忽略警告 C4267
if(MSVC)
    add_compile_options(/wd4267)
endif()

# 根目录下所以的源文件
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "*.cpp")
foreach(SRC_FILE ${SRC_FILES})
    get_filename_component(SRC_NAME ${SRC_FILE} NAME_WE)
    message(STATUS "Adding source file: ${SRC_NAME}")
    add_executable(${SRC_NAME} ${SRC_FILE})
endforeach()



