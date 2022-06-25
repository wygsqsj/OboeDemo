cmake_minimum_required(VERSION 3.4.1)

project("oboedemo")

#导入当前的cpp
file(GLOB app_soures oboe/*.cpp)

add_library(
        native-lib
        SHARED
        ${app_soures})

find_library(log-lib
        log)

#导入jnilibs下的so
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/Users/mac/studioWorkPlace/custom/OboeDemo/app/src/main/jniLibs/${ANDROID_ABI}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}")
add_library(
        libAudioAcoustic
        SHARED
        IMPORTED
)

add_library(
        libAudioAnalyze
        SHARED
        IMPORTED
)

add_library(
        libFluxLib
        SHARED
        IMPORTED
)

set(
        lib_dir
        /Users/mac/studioWorkPlace/custom/OboeDemo/app/src/main/jniLibs
)
set_target_properties(
        libAudioAcoustic
        PROPERTIES IMPORTED_LOCATION
        ${lib_dir}/${ANDROID_ABI}/libAudioAcoustic.so
)

set_target_properties(
        libAudioAnalyze
        PROPERTIES IMPORTED_LOCATION
        ${lib_dir}/${ANDROID_ABI}/libAudioAnalyze.so
)

set_target_properties(
        libFluxLib
        PROPERTIES IMPORTED_LOCATION
        ${lib_dir}/${ANDROID_ABI}/libFluxLib.so
)

#导入识别库头文件
include_directories(${CMAKE_SOURCE_DIR}/include/)

set(OBOE_DIR /Users/mac/studioWorkPlace/custom/oboe)

add_subdirectory(${OBOE_DIR} ./oboe)

include_directories(${OBOE_DIR}/include)

#target_link_libraries(native-lib log)

target_link_libraries(
        native-lib
        ${log-lib}
        libAudioAcoustic
        libAudioAnalyze
        libFluxLib
        oboe
)