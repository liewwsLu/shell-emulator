@echo off
cd /d "%~dp0.."
copy /y tests\vfs\deep.xml bin\deep_before.xml >nul
echo ===== mv and touch on deep.xml, changes are made only in memory =====
bin\emulator.exe --vfs tests\vfs\deep.xml --script tests\scripts\stage5.txt
echo.
echo ===== the physical VFS file is not modified =====
fc /b bin\deep_before.xml tests\vfs\deep.xml >nul
if errorlevel 1 (echo tests\vfs\deep.xml CHANGED) else (echo tests\vfs\deep.xml is unchanged)
echo.
pause
