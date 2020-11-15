#!/bin/bash
g++ \
\
-std=c++17 \
\
-D __linux__ \
-D JASONPP_RENDER_SORTED \
\
../Tubio/main.cpp \
../Tubio/ConsoleManager.cpp \
../Tubio/DownloadManager.cpp \
../Tubio/FileSystem.cpp \
../Tubio/Framework.cpp \
../Tubio/HttpServer.cpp \
../Tubio/Logger.cpp \
../Tubio/LogHistory.cpp \
../Tubio/RestQueryHandler.cpp \
../Tubio/RestResponseTemplates.cpp \
../Tubio/XGConfig.cpp \
../Tubio/XGControl.cpp \
../Tubio/Updater.cpp \
../Tubio/Idler.cpp \
\
\
../Tubio/external_dependencies/casenta/mongoose/mongoose.c \
../Tubio/external_dependencies/leonetienne/JasonPP/JasonPP.cpp \
\
-lpthread \
\
-w \
-o ./tubio.out
