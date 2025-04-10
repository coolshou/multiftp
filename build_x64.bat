@ECHO OFF
setlocal EnableDelayedExpansion


echo "Check Qt build environment"
echo "%PATH%" | findstr /c:"Qt"
if !errorlevel! neq 0 (
    set currentDir=%cd%
    set "driveLetter=%currentDir:~0,2%"
    echo set QT msvc2022_64 PATH
    REM this will enter another comspec!
    %comspec% /k "C:\Qt\6.8.0\msvc2022_64\bin\qtenv2.bat"
    %driveLetter%
)
echo "Check Visual Studio 2022 build environment"
echo "%PATH%" | findstr /c:"Visual Studio"
if !errorlevel! neq 0 (
    echo set Visual Studio 2022 Community PATH
    %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

echo qmake...
qmake
echo nmake...
nmake
windeployqt multiftp_x86_64\multiftp.exe
