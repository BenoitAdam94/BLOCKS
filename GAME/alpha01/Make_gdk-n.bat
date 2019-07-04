echo off

set PROG=main

set CC=gbdk-n\bin\gbdk-n-compile.bat
set LD=gbdk-n\bin\gbdk-n-link.bat
set MR=gbdk-n\bin\gbdk-n-make-rom.bat

:: Clean
if "%1" == "clean" (
    del *.rel
    del *.lst
    del *.sym
    del *.asm
    del %PROG%.gb
    del %PROG%.ihx
    del %PROG%.map
    del %PROG%.sym
    del %PROG%.noi
    del %PROG%.lk
    goto end
)

:: Check gbdk-n
if not exist gbdk-n (
    echo "gbdk-n folder not found in current directory. Please download it."
    pause
    goto end
)

:: Compile gbdk-n if not already done
if not exist gbdk-n\lib\gb.lib (
    cd gbdk-n
    call Make.bat
    cd ..
)

:: Build
for %%A in (*.c) do (
    if exist %%~nA.rel del %%~nA.rel
    echo Compiling: %%~nxA && call %CC% -o %%~nA.rel -c %%~nA.c
    if not exist %%~nA.rel echo. && echo Build failed! && echo. && pause && goto end
)

setlocal enabledelayedexpansion
set REL_LIST=
for %%A in (*.rel) do set REL_LIST=!REL_LiST! %%A

if exist %PROG%.ihx del %PROG%.ihx
echo Linking: %PROG%.ihx
call %LD% -o %PROG%.ihx %REL_LIST%
if not exist %PROG%.ihx echo. && echo Build failed! && echo. && pause && goto end

if exist %PROG%.gb del %PROG%.gb
echo Building ROM: %PROG%.gb
call %MR% %PROG%.ihx %PROG%.gb
if not exist %PROG%.gb echo. && echo Build failed! && echo. && pause && goto end

:end
