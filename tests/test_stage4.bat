@echo off
cd /d "%~dp0.."
echo ===== ls, cd, uname, cal, rev on deep.xml =====
bin\emulator.exe --vfs tests\vfs\deep.xml --script tests\scripts\stage4.txt
echo.
pause
