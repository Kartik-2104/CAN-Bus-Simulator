@echo off
g++ -std=c++17 main.cpp CANFrame.cpp ECU.cpp CANBus.cpp CANAnalyzer.cpp -o can_simulator.exe
if %errorlevel% neq 0 pause
