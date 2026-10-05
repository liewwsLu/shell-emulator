@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion
cd /d "%~dp0"

if not exist bin mkdir bin

taskkill /F /IM emulator.exe >nul 2>&1
timeout /t 1 /nobreak >nul

if exist bin\emulator.exe del /F /Q bin\emulator.exe >nul 2>&1
if exist bin\emulator.exe (
    echo Не удалось удалить bin\emulator.exe - файл занят.
    pause
    exit /b 1
)

set SRC=
for %%f in (src\*.cpp) do set SRC=!SRC! %%f

echo Сборка проекта...
g++ -std=c++17 -o bin/emulator.exe !SRC!

if errorlevel 1 (
    echo.
    echo Ошибка сборки. Смотрите сообщения компилятора выше.
    pause
    exit /b 1
)

echo Сборка завершена.
echo.

set VFS=
if exist tests\vfs\deep.xml set VFS=--vfs tests\vfs\deep.xml

bin\emulator.exe !VFS! %*