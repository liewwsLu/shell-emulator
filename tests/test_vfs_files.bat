@echo off
cd /d "%~dp0.."
echo ===== VFS with several files in the root =====
bin\emulator.exe --vfs tests\vfs\files.xml --script tests\scripts\vfs_tour.txt
echo.
pause
