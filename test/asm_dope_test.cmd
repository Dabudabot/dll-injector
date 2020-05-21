@echo off

set CURRENT_DIRECTORY=%cd%
if "%1"=="" goto :error
set PATH_TO_BIN=%1%
shift

if not exist %PATH_TO_BIN% goto :error

cd %PATH_TO_BIN%

if not exist asm_dope.exe goto :error

echo -
echo ---------------------------------------------------------------------
echo STARTING: asm_dope_test %PATH_TO_BIN%       
echo ---------------------------------------------------------------------
echo -

call asm_dope.exe 1 + 3
if not %errorlevel% == 4 goto :error
call asm_dope.exe 3 - 1
if not %errorlevel% == 2 goto :error
call asm_dope.exe 3 * 5
if not %errorlevel% == 15 goto :error

cd %CURRENT_DIRECTORY%

echo -
echo ---------------------------------------------------------------------
echo SUCCESS: asm_dope_test %PATH_TO_BIN%              
echo ---------------------------------------------------------------------
echo -

goto :end

:error
cd %CURRENT_DIRECTORY%
echo *
echo ********************************************
echo ERROR asm_dope_test
echo ********************************************
echo *
EXIT /B 1

:end
EXIT /B 0