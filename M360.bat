@echo off
setlocal EnableDelayedExpansion
REM ============================================================
REM  M360 Life - TEK DOSYA (her iki PC)
REM  Cift tik = git pull + bu PC hazirlik
REM  Oyna:    M360.bat oyna
REM ============================================================
cd /d "%~dp0"
title M360
echo.

if /I "%~1"=="oyna" goto OYNA

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0tools\m360-otomatik.ps1"
set ERR=!ERRORLEVEL!
echo.
if not "!ERR!"=="0" (
  echo HATA kodu: !ERR!
  pause
  exit /b !ERR!
)
echo.
echo Hazir. Pencere 6 sn sonra kapanir...
timeout /t 6 >nul
exit /b 0

:OYNA
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0tools\m360-otomatik.ps1" -Oyna
set ERR=!ERRORLEVEL!
if not "!ERR!"=="0" (
  echo.
  echo HATA kodu: !ERR!
  pause
  exit /b !ERR!
)
exit /b 0
