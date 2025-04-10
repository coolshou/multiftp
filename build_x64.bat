@ECHO OFF
setlocal EnableDelayedExpansion

echo "Check Visual Studio 2022 build environment"
echo "%PATH%" | findstr /c:"Visual Studio"
if !errorlevel! neq 0 (
    echo set Visual Studio 2022 Community PATH
    echo %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

echo "Check Qt build environment"
echo "%PATH%" | findstr /c:"Qt"
if !errorlevel! neq 0 (
    set currentDir=%cd%
    set "driveLetter=%currentDir:~0,2%"
    echo set QT msvc2022_64 PATH
    REM this will enter another comspec!
    echo "C:\Qt\6.8.3\msvc2022_64\bin\qtenv2.bat"
    "C:\Qt\6.8.3\msvc2022_64\bin\qtenv2.bat"
    %driveLetter%
)

echo qmake...
qmake
echo nmake...
nmake
echo multiftp_x86_64\multiftp.exe
windeployqt multiftp_x86_64\multiftp.exe
