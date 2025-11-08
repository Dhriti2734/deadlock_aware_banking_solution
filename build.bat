@echo off
REM Build script for Deadlock Aware Banking System
REM Windows MinGW GCC

echo ========================================
echo Deadlock Aware Banking System
echo Build Script for Windows MinGW
echo ========================================
echo.

REM Check if g++ is available
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: g++ compiler not found!
    echo Please ensure MinGW is installed and in your PATH.
    echo.
    pause
    exit /b 1
)

echo Compiler found: g++
echo.

REM Clean previous build
echo Cleaning previous build...
if exist *.o del *.o
if exist BankingSystem.exe del BankingSystem.exe
echo.

REM Compile all source files
echo Compiling source files...
echo.

echo [1/8] Compiling Account.cpp...
g++ -std=c++17 -O2 -c Account.cpp -o Account.o
if errorlevel 1 goto :error

echo [2/8] Compiling Transaction.cpp...
g++ -std=c++17 -O2 -c Transaction.cpp -o Transaction.o
if errorlevel 1 goto :error

echo [3/8] Compiling Logger.cpp...
g++ -std=c++17 -O2 -c Logger.cpp -o Logger.o
if errorlevel 1 goto :error

echo [4/8] Compiling DeadlockHandler.cpp...
g++ -std=c++17 -O2 -c DeadlockHandler.cpp -o DeadlockHandler.o
if errorlevel 1 goto :error

echo [5/8] Compiling Scheduler.cpp...
g++ -std=c++17 -O2 -c Scheduler.cpp -o Scheduler.o
if errorlevel 1 goto :error

echo [6/8] Compiling TransactionManager.cpp...
g++ -std=c++17 -O2 -c TransactionManager.cpp -o TransactionManager.o
if errorlevel 1 goto :error

echo [7/8] Compiling BankingSystem.cpp...
g++ -std=c++17 -O2 -c BankingSystem.cpp -o BankingSystem.o
if errorlevel 1 goto :error

echo [8/8] Compiling main.cpp...
g++ -std=c++17 -O2 -c main.cpp -o main.o
if errorlevel 1 goto :error

echo.
echo Linking executable...
g++ -o BankingSystem.exe Account.o Transaction.o Logger.o DeadlockHandler.o Scheduler.o TransactionManager.o BankingSystem.o main.o -pthread
if errorlevel 1 goto :error

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.
echo Executable created: BankingSystem.exe
echo.
echo To run the program, execute:
echo     BankingSystem.exe
echo.
pause
exit /b 0

:error
echo.
echo ========================================
echo BUILD FAILED!
echo ========================================
echo.
echo Please check the error messages above.
echo.
pause
exit /b 1