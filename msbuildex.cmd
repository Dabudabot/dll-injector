@echo off

set CURRENT_DIRECTORY=%cd%
if "%1"=="" goto :error

set PATH_TO_PROJECT=%1%
shift
if "%1"=="" goto :error
set PROJECT_NAME=%1%
shift
if "%1"=="" goto :error
set CONFIG=%1%
shift
if "%1"=="" goto :error
set PLATFORM=%1%
shift

cd %PATH_TO_PROJECT%

echo -
echo ---------------------------------------------------------------------
echo STARTING: Building %PROJECT_NAME%              
echo ---------------------------------------------------------------------
echo -

call msbuild %PROJECT_NAME%.vcxproj /p:configuration=%CONFIG% /p:platform=%PLATFORM%

if errorlevel 1 goto :error

cd %CURRENT_DIRECTORY%

echo -
echo ---------------------------------------------------------------------
echo SUCCESS: Building %PROJECT_NAME%              
echo ---------------------------------------------------------------------
echo -

goto :end

:error
cd %CURRENT_DIRECTORY%
echo *
echo ********************************************
echo ERROR msbuildex.cmd 
echo ********************************************
echo *
EXIT /B 1

:end
EXIT /B 0