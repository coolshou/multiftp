@ECHO OFF
setlocal enabledelayedexpansion

echo "%PATH%" | findstr /c:"NSIS" >nul
if "%errorlevel%"=="1" (
    echo setting NSIS PATH
    set "PATH=C:\Program Files (x86)\NSIS\;%PATH%"
)

REM # get version from version.h
set "file=src\versions.h"
set "multiftp_pattern=#define MULTIFTP_VERSION"
set "multiftpfile_pattern=#define MULTIFTPFILE_VERSION"

for /f "tokens=3 delims=\ " %%i in ('findstr /c:"%multiftp_pattern%" "%file%"') do (
    set APPVERSION=%%i
    echo APPVERSION:!APPVERSION!
)
for /f "tokens=3 delims=\ " %%i in ('findstr /c:"%multiftpfile_pattern%" "%file%"') do (
    set APPFILEVERSION=%%i
    echo APPFILEVERSION:!APPFILEVERSION!
)

IF DEFINED APPVERSION (
    echo create multiftp setup...
    echo "makensis.exe /DAPPVERSION=%APPVERSION% /DAPPFileVersion=%APPFILEVERSION% /DWIN64 /V4 multiftp.nsi"
    makensis.exe /DAPPVERSION=%APPVERSION% /DAPPFileVersion=%APPFILEVERSION% /DWIN64 /V4 multiftp.nsi

) ELSE (
    echo "Did not get APPVERSION"
)
