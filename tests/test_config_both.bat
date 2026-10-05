@echo off
cd /d "%~dp0.."
echo ===== both options =====
bin\emulator.exe --vfs tests\vfs\deep.xml --script tests\scripts\stage2.txt
echo.
echo ===== both options in reverse order =====
bin\emulator.exe --script tests\scripts\stage2.txt --vfs tests\vfs\deep.xml
echo.
pause
