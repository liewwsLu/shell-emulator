@echo off
cd /d "%~dp0.."
echo ===== script file does not exist =====
echo exit| bin\emulator.exe --script tests\scripts\missing.txt
echo.
echo ===== unknown option =====
bin\emulator.exe --color red
echo.
echo ===== --vfs without value =====
bin\emulator.exe --vfs
echo.
echo ===== --script without value =====
bin\emulator.exe --vfs tests\vfs\deep.xml --script
echo.
pause
