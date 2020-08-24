@echo off

cmd /C %BUILD_PATH%\php.exe -f %PHP_SRC_DIR%\run-tests.php %PHP_SRC_DIR%\ext\bqen\tests -p %BUILD_PATH%\php.exe -d extension=%BUILD_PATH%\php_bqen.dll

exit /B %errorlevel%
