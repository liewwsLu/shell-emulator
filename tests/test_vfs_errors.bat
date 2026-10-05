@echo off
cd /d "%~dp0.."
echo ===== VFS file does not exist =====
bin\emulator.exe --vfs tests\vfs\missing.xml
echo.
echo ===== broken XML =====
bin\emulator.exe --vfs tests\vfs\broken.xml
echo.
echo ===== invalid base64 content =====
bin\emulator.exe --vfs tests\vfs\bad_base64.xml
echo.
echo ===== unknown element =====
bin\emulator.exe --vfs tests\vfs\unknown_element.xml
echo.
pause
