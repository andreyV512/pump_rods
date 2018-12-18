git describe --tags --long --always>NumVer.txt
fc NumVer.txt NumVerOld.txt > nul
if ERRORLEVEL 1 goto Different
if ERRORLEVEL 2 goto Different
if ERRORLEVEL 0 goto NoDifferent
:Different
call :Gen
:NoDifferent
if not exist Version.h call :Gen
if exist NumVerOld.txt del NumVerOld.txt
RENAME NumVer.txt NumVerOld.txt
goto :EOF

:Gen
echo #pragma once > Version.h
set /p file=<NumVer.txt
echo #define VERSION() L"Cборка %file%">> Version.h
goto :EOF


