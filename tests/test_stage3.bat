@echo off
cd /d "%~dp0.."
copy /y tests\vfs\deep.xml bin\vfs_copy.xml >nul
echo ===== commands of stages 1-3 on a copy of deep.xml =====
bin\emulator.exe --vfs bin\vfs_copy.xml --script tests\scripts\stage3.txt
echo.
echo ===== physical VFS file after vfs-init =====
type bin\vfs_copy.xml
echo.
pause
