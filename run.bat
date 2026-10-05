@echo off
cd /d "%~dp0"
if not exist bin\emulator.exe (
    echo bin\emulator.exe not found. Build the project first, see README.md
    pause
    exit /b 1
)
bin\emulator.exe --vfs tests\vfs\deep.xml %*
