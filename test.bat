@echo off
SETLOCAL
set next=0


REM -- TEST: NO ARGS --
type     >type_cmd.1 2>NUL
type.exe >type_exe.1 2>NUL
type     2>type_cmd.2 >NUL
type.exe 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST1


REM -- TEST: HELP SWITCH --
type/?     >type_cmd.1 2>NUL
type.exe/? >type_exe.1 2>NUL
type/?     2>type_cmd.2 >NUL
type.exe/? 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST2


REM -- TEST: HELP SWITCH W/ OTHER ARGS --
type /T /? /p     >type_cmd.1 2>NUL
type.exe /T /? /p >type_exe.1 2>NUL
type /T /? /p     2>type_cmd.2 >NUL
type.exe /T /? /p 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST3


REM -- TEST: BOGUS SWITCHES --
type /T /p     >type_cmd.1 2>NUL
type.exe /T /p >type_exe.1 2>NUL
type /T /p     2>type_cmd.2 >NUL
type.exe /T /p 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST4


REM -- TEST: BAD FILENAME --
type badfilename     >type_cmd.1 2>NUL
type.exe badfilename >type_exe.1 2>NUL
type badfilename     2>type_cmd.2 >NUL
type.exe badfilename 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST5


REM -- TEST: BAD FILENAME W/ MULTPILE FILES --
type badfile1 badfile2     >type_cmd.1 2>NUL
type.exe badfile1 badfile2 >type_exe.1 2>NUL
type badfile1 badfile2     2>type_cmd.2 >NUL
type.exe badfile1 badfile2 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST6


REM -- TEST: MULTIPLE FILES --
type .gitattributes .gitignore     >type_cmd.1 2>NUL
type.exe .gitattributes .gitignore >type_exe.1 2>NUL
type .gitattributes .gitignore     2>type_cmd.2 >NUL
type.exe .gitattributes .gitignore 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST7


GOTO EXIT


:ERRORCHECK
set /a next=%next%+1

fc.exe /b type_cmd.1 type_exe.1 >NUL
if ERRORLEVEL 1 GOTO ERROREXIT
fc.exe /b type_cmd.2 type_exe.2 >NUL
if ERRORLEVEL 1 GOTO ERROREXIT

GOTO AFTERTEST%next%


:ERROREXIT
echo Error: Test %next% - Mismatch in type_cmd and type_exe files.
exit /b 1
@echo on


:EXIT
del type_cmd.* type_exe.*
echo All tests passed.
@echo on