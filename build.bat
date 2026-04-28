@echo off
setlocal enabledelayedexpansion
cls
echo ===================================================
echo   COMPILING: DATA STRUCTURE VISUALIZER
echo   Student: Le Tien Binh - 25125007
echo ===================================================
echo.

:: Delete old executable if it exists
if exist Visualizer.exe del Visualizer.exe

:: Gather all .cpp files from the src directory
echo [1/2] Gathering source files...
set "SRC_FILES="
for %%f in (src\*.cpp) do set "SRC_FILES=!SRC_FILES! %%f"

:: Compile the project
echo [2/2] Compiling and linking with SFML and Windows APIs...
g++ !SRC_FILES! -std=c++17 -Iinclude -Iheaders -Llib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lopengl32 -lcomdlg32 -o Visualizer.exe

:: Check the result
if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Visualizer.exe has been created!
    echo Starting the application...
    echo.
    .\Visualizer.exe
) else (
    echo.
    echo [FAILED] Compilation error occurred. Please check the terminal for details.
)
pause