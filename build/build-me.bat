@setlocal
@set DOINST=0
@set TMPVER=1
@set TMPPRJ=tidy-view
@set TMPSRC=..
@set TMPBGN=%TIME%
@set TMPINS=..\..\software
@set DOTINST=0
@set TMPLOG=bldlog-1.txt

@set TMPOPTS=-DCMAKE_INSTALL_PREFIX=%TMPINS%
@REM set TMPOPTS=%TMPOPTS% -DBUILD_SHARED_LIB=ON
@if NOT EXIST %TMPINS%\nul goto NOINS

:RPT
@if "%~1x" == "x" goto GOTCMD
@set TMPOPTS=%TMPOPTS% %1
@shift
@goto RPT
:GOTCMD

@call chkmsvc %TMPPRJ%
@REM call chkbranch master

@echo Build %DATE% %TIME% > %TMPLOG%

@if NOT EXIST %TMPSRC%\nul goto NOSRC

@echo Build source %TMPSRC%... all output to build log %TMPLOG%
@echo Build source %TMPSRC%... all output to build log %TMPLOG% >> %TMPLOG%

@if EXIST build-cmake.bat (
@call build-cmake >> %TMPLOG%
)

@if NOT EXIST %TMPSRC%\CMakeLists.txt goto NOCM

cmake %TMPSRC% %TMPOPTS% >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR1

cmake --build . --config Debug  >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR2

cmake --build . --config Release  >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR3

@fa4 "***" %TMPLOG%
@call elapsed %TMPBGN%
@echo Appears a successful build... see %TMPLOG%

@echo.
@if NOT "%DOINST%x" == "1x" (
@echo No install at this time. Set DOINST=1
@echo.
@goto END
)

@echo Continue with install? Only Ctrl+c aborts...

@pause

cmake --build . --config Debug  --target INSTALL >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR4
@REM if EXIST install_manifest.txt (
@REM copy install_manifest.txt install_manifest_dbg.txt >nul
@REM echo. >> %TMPINS%/installed.txt
@REM echo = %TMPRJ% Debug install %DATE% %TIME% >> %TMPINS%/installed.txt
@REM type install_manifest.txt >> %TMPINS%/installed.txt
@REM )

cmake --build . --config Release  --target INSTALL >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR5
@REM if EXIST install_manifest.txt (
@REM copy install_manifest.txt install_manifest_rel.txt >nul
@REM echo. >> %TMPINS%/installed.txt
@REM echo = %TMPRJ% Release install %DATE% %TIME% >> %TMPINS%/installed.txt
@REM type install_manifest.txt >> %TMPINS%/installed.txt
@REM )

@fa4 " -- " %TMPLOG%

@call elapsed %TMPBGN%
@echo All done... see %TMPLOG%

@goto END

:NOINS
@echo Can NOT locate 3rdParty %TMPINS%! *** FIX ME ***
@goto ISERR

:NOSRC
@echo Can NOT locate source %TMPSRC%! *** FIX ME ***
@echo Can NOT locate source %TMPSRC%! *** FIX ME *** >> %TMPLOG%
@goto ISERR

:NOCM
@echo Can NOT locate %TMPSRC%\CMakeLists.txt!
@echo Can NOT locate %TMPSRC%\CMakeLists.txt! >> %TMPLOG%
@goto ISERR

:ERR1
@echo cmake configuration or generations ERROR
@echo cmake configuration or generations ERROR >> %TMPLOG%
@goto ISERR

:ERR2
@echo ERROR: Cmake build Debug FAILED!
@echo ERROR: Cmake build Debug FAILED! >> %TMPLOG%
@goto ISERR

:ERR3
@echo ERROR: Cmake build Release FAILED!
@echo ERROR: Cmake build Release FAILED! >> %TMPLOG%
@goto ISERR

:ERR4
@echo ERROR: Install Debug FAILED!
@echo ERROR: Install Debug  FAILED! >> %TMPLOG%
@goto ISERR

:ERR5
@echo ERROR: Install Release FAILED!
@echo ERROR: Install Release  FAILED! >> %TMPLOG%
@goto ISERR

:ISERR
@echo See %TMPLOG% for details...
@endlocal
@exit /b 1

:END
@endlocal
@exit /b 0

@REM eof
