cmake_minimum_required(VERSION 3.16)
project(Tubio)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add StringTools src dir to include dir list
include_directories(./external_dependencies/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Add absolutely kawaii sources to Tubio <3
add_executable(Tubio
        main.cpp
        ConsoleManager.cpp
        ConsoleManager.h
        DownloadManager.cpp
        DownloadManager.h
        FileSystem.cpp
        FileSystem.h
        Framework.cpp
        Framework.h
        HttpServer.cpp
        HttpServer.h
        Idler.cpp
        Idler.h
        LogHistory.cpp
        LogHistory.h
        LogTypes.h
        Logger.cpp
        Logger.h
        RestQueryHandler.cpp
        RestQueryHandler.h
        RestResponseTemplates.cpp
        RestResponseTemplates.h
        TimeUnits.h
        Updater.cpp
        Updater.h
        Version.h
        XGConfig.cpp
        XGConfig.h
        XGControl.cpp
        XGControl.h
        external_dependencies/casenta/mongoose/mongoose.c
        external_dependencies/casenta/mongoose/mongoose.h
        external_dependencies/leonetienne/JasonPP/JasonPP.hpp
        external_dependencies/leonetienne/JasonPP/JasonPP.cpp
)

