cmake_minimum_required(VERSION 3.17)

project(RaymarchingOptimizations VERSION 1.0 LANGUAGES CXX)
set(TARGET_NAME RaymarchingOptimizations)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(FetchContent)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ProjectFiles)

set(PROJECT_SOURCE_FILES ${PROJECT_DIR}/main.cpp
    ${PROJECT_DIR}/Vector3.h
    ${PROJECT_DIR}/Vector3.cpp

    ${PROJECT_DIR}/Vector3.h
    ${PROJECT_DIR}/Vector4.cpp

    ${PROJECT_DIR}/Timer.h
    ${PROJECT_DIR}/Timer.cpp

    ${PROJECT_DIR}/SDFObjects.h
    ${PROJECT_DIR}/SDFObjects.cpp

    ${PROJECT_DIR}/SdEngine.h
    ${PROJECT_DIR}/SdEngine.cpp

    ${PROJECT_DIR}/Scene.h
    ${PROJECT_DIR}/Scene.cpp

    ${PROJECT_DIR}/Renderer.h
    ${PROJECT_DIR}/Renderer.cpp

    ${PROJECT_DIR}/Matrix.h
    ${PROJECT_DIR}/Matrix.cpp

    ${PROJECT_DIR}/Material.h
    
    ${PROJECT_DIR}/ColorRGB.h
    ${PROJECT_DIR}/Camera.h
    ${PROJECT_DIR}/BRDFs.h
    ${PROJECT_DIR}/DataTypes.h
)

FetchContent_Declare(
    SDL2
    URL https://www.libsdl.org/release/SDL2-devel-2.28.5-VC.zip 
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads 
)

FetchContent_GetProperties(SDL2)
if(NOT SDL2_POPULATED)
    FetchContent_Populate(SDL2)
    set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)

    set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
    set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
endif()

add_executable(${TARGET_NAME} ${PROJECT_SOURCE_FILES})

target_include_directories(${TARGET_NAME} PRIVATE
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" ${CMAKE_BINARY_DIR}
)