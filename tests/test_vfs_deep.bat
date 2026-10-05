@echo off
cd /d "%~dp0.."
echo ===== VFS with more than 3 levels of directories =====
bin\emulator.exe --vfs tests\vfs\deep.xml --script tests\scripts\vfs_tour.txt
echo.
pause
