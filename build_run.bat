@echo off
title build and run zero sum

echo ============================
echo Cleaning old files...
echo ============================
make clean

echo ============================
echo Compiling application...
echo ============================
make all
if %ERRORLEVEL% NEQ 0 goto build_fail

echo ============================
echo Launching application...
echo ============================
echo.

make run

goto end

:build_fail
echo.
echo [ERROR] Build failed with error code %ERRORLEVEL%.
exit /b %ERRORLEVEL%

:end
echo.
echo [SUCCESS] Application closed.
exit /b 0