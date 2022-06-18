cmake_minimum_required(VERSION 3.4.1)

project("oboedemo")

#导入当前的cpp
file(GLOB app_soures opoe/*.cpp)

add_library(
        native-lib
        SHARED
        ${app_soures})

find_library(log-lib
        log)

set(OBOE_DIR  /Users/mac/studioWorkPlace/custom/oboe)

add_subdirectory(${OBOE_DIR} ./oboe)

include_directories(${OBOE_DIR}/include)

target_link_libraries(native-lib log)

target_link_libraries(
        native-lib
        ${log-lib}
        oboe
)