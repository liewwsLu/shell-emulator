@echo off
cd /d "%~dp0.."
echo ===== minimal VFS: only the root directory =====
bin\emulator.exe --vfs tests\vfs\minimal.xml --script tests\scripts\vfs_tour.txt
echo.
pause
