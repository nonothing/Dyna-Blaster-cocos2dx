@echo off
@set APKFILE=Dyna_Blaster-debug.apk

@call adb install -r bin/%APKFILE%
@if ERRORLEVEL 1 goto ERROR_END

@goto END

:ERROR_END
@echo *********************************************************
@echo *** Errors was detected! Install script is terminated! ***
@echo *********************************************************
@exit /b 1

:END

set APKFILE=