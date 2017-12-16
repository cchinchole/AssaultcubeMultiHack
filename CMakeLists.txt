cmake_minimum_required(VERSION 3.9)
project(assaultCubeDLL)
set(CMAKE_CXX_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_library(assaultCubeDLL SHARED
        Aimbot.cpp
        Aimbot.h
        dllmain.cpp
        Draw.cpp
        Draw.h
        Entity.cpp
        Entity.h
        Esp.cpp
        Esp.h
        Math.cpp
        Math.h
        Menu.cpp
        Menu.h
        Mod.cpp
        Mod.h
        Offsets.h
        ProcessMemoryWriter.cpp
        ProcessMemoryWriter.h
        Rage.h
        Source.cpp
        stdafx.cpp
        stdafx.h
        targetver.h
        Debug/assaultCubeDLL.cpp Debug/assaultCubeDLL.h)
#Detours-master/lib.x86/detours.lib
include_directories("e:/vcc_libs/glut/")
include_directories("e:/vcc_libs/Detours-master/include/")
target_link_libraries(assaultCubeDLL E:/vcc_libs/glut/glut32.lib)
target_link_libraries(assaultCubeDLL e:/vcc_libs/Detours-master/lib.x86/detours.lib)

#find_package (GLUT REQUIRED)
#include(FindPkgConfig)
#find_package(glut REQUIRED)
#include_directories(${GLUT_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARY})

#find_package(detours REQUIRED)
#include_directories(${DETOURS_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${DETOURS_LIBRARY})