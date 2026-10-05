@echo off
cd /d "%~dp0.."
echo ===== no options =====
echo exit| bin\emulator.exe
echo.
echo ===== only --vfs =====
echo exit| bin\emulator.exe --vfs tests\vfs\deep.xml
echo.
echo ===== only --script =====
bin\emulator.exe --script tests\scripts\stage2.txt
echo.
pause
