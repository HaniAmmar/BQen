@echo off

set PHP_ROOT_DIR=C:\php-dev

if exist "custom_build.bat" (
    call custom_build.bat
    if %errorlevel% neq 0 exit /B 1
)

if "%PHP_CONFIG%" equ "" (
    set PHP_CONFIG= --disable-all ^
                    --enable-cli ^
                    --enable-bqen=shared
)

rem PHP version number
if "%PHP_VER%" equ ""        set PHP_VER=7.4.3
if "%PHP_SDK_VER%" equ ""    set PHP_SDK_VER=2.2.0
rem Set to zero to disable Thread safe
if "%PHP_IS_TS%" equ ""      set PHP_IS_TS=1
if "%VS_BUILD%" equ ""       set VS_BUILD=vs16
if "%WIN_PLATFORM%" equ ""   set WIN_PLATFORM=x64
if "%BUILD_TYPE%" equ ""     set BUILD_TYPE=Release

REM set INSTALL_PHP=1
REM set GET_PHP_DEP=1
REM set NATIVE=AVX

set PHP_DEP_BRANCH=master

if "%NATIVE%" neq "" (
    set PHP_CONFIG=%PHP_CONFIG% --enable-native-intrinsics=%NATIVE%
)

if "%PHP_IS_TS%" equ "0" (
    set PHP_CONFIG=%PHP_CONFIG% --disable-zts
    set PHP_TS=
) else (
    set PHP_TS=_TS
)

set PHP_SRC_DIR=%PHP_ROOT_DIR%\php-src-%PHP_VER%
set PHP_SDK_DIR=%PHP_ROOT_DIR%\php-sdk-%PHP_SDK_VER%
set PHP_DEP_DIR=%PHP_ROOT_DIR%\php-dep-%PHP_DEP_BRANCH%
set PHP_BQEN_EXT_DIR=%PHP_SRC_DIR%\ext\bqen

if "%WIN_PLATFORM%" equ "x64" (
    set BUILD_PATH=%PHP_SRC_DIR%\%WIN_PLATFORM%\%BUILD_TYPE%%PHP_TS%
) else (
    set BUILD_PATH=%PHP_SRC_DIR%\%BUILD_TYPE%%PHP_TS%
)

set BUILD_TASK=%PHP_SRC_DIR%\build_task.bat

echo Getting Qentem
cmd /C git submodule update --init
if %errorlevel% neq 0 exit /B 7

if not exist "%PHP_SRC_DIR%\.git" (
    echo Cloning PHP reposotory

    if "%PHP_VER%" equ "master" (
        cmd /C git clone --depth=1 --branch=master https://github.com/php/php-src.git %PHP_SRC_DIR%
    ) else (
        cmd /C git clone --depth=1 --branch=PHP-%PHP_VER% https://github.com/php/php-src.git %PHP_SRC_DIR%
    )

    if not exist "%PHP_SRC_DIR%\.git" exit /B 2

) else (
    if exist "%PHP_BQEN_EXT_DIR%" (
        rmdir /Q /S %PHP_BQEN_EXT_DIR%
    )
)

if not exist "%PHP_SDK_DIR%\.git" (
    echo.
    echo Cloning PHP SDK %PHP_SDK_VER%
    cmd /C git clone --depth=1 --branch=php-sdk-%PHP_SDK_VER% https://github.com/Microsoft/php-sdk-binary-tools.git %PHP_SDK_DIR%

    if not exist "%PHP_SDK_DIR%\.git" (
        exit /B 2
    )
)

xcopy /E /Q /Y /I . %PHP_BQEN_EXT_DIR% >NUL 2>NUL

if exist "%BUILD_TASK%" (
    del %BUILD_TASK%
    if %errorlevel% neq 0 exit /B 2
)

echo @echo off >> %BUILD_TASK%

if "%GET_PHP_DEP%" equ "1" (
    echo cmd /c %PHP_SDK_DIR%\bin\phpsdk_deps.bat -u -n  -s stable -b %PHP_DEP_BRANCH% -d %PHP_DEP_DIR% >> %BUILD_TASK%
    echo if %errorlevel% neq 0 exit /b 4 >> %BUILD_TASK%

    set PHP_CONFIG=%PHP_CONFIG% --with-php-build=%PHP_DEP_DIR%
)

echo cd /D %PHP_SRC_DIR% >> %BUILD_TASK%
echo if %errorlevel% neq 0 exit /B 2 >> %BUILD_TASK%
echo cmd /C buildconf.bat --force >> %BUILD_TASK%
echo if %errorlevel% neq 0 exit /B 2 >> %BUILD_TASK%
echo cmd /C configure.bat %PHP_CONFIG% >> %BUILD_TASK%
echo if %errorlevel% neq 0 exit /B 2 >> %BUILD_TASK%

echo cmd /C nmake /NOLOGO >> %BUILD_TASK%
echo if %errorlevel% neq 0 exit /B 2 >> %BUILD_TASK%


if "%INSTALL_PHP%" equ "1" (
    echo cmd /C nmake install >> %BUILD_TASK%
)

echo.
echo Building
cmd /C %PHP_SDK_DIR%\phpsdk-%VS_BUILD%-%WIN_PLATFORM%.bat -t %BUILD_TASK%

if "%AFTER_BUILD%" neq "" cmd /C %AFTER_BUILD%

REM cmd /C win\test

exit /B %errorlevel%
