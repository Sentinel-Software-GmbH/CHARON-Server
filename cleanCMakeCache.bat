@echo off
SET CMakeCache="build\CMakeDebugBuild\CMakeCache.txt"
SET CMakeCacheFolder="build\CMakeDebugBuild\CMakeFiles"

IF EXIST %CMakeCache% DEL /F %CMakeCache% 
IF EXIST %CMakeCacheFolder% RMDIR /s /q %CMakeCacheFolder% 