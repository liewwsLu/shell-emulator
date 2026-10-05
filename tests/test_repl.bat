@echo off
cd /d "%~dp0.."
echo ===== REPL: commands from tests\scripts\repl.txt =====
bin\emulator.exe < tests\scripts\repl.txt
echo.
pause
