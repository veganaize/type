@echo off
SETLOCAL
set next=0


type     >type_cmd.1 2>NUL
type.exe >type_exe.1 2>NUL
type     2>type_cmd.2 >NUL
type.exe 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST1


type/?     >type_cmd.1 2>NUL
type.exe/? >type_exe.1 2>NUL
type/?     2>type_cmd.2 >NUL
type.exe/? 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST2


type /T /? /p     >type_cmd.1 2>NUL
type.exe /T /? /p >type_exe.1 2>NUL
type /T /? /p     2>type_cmd.2 >NUL
type.exe /T /? /p 2>type_exe.2 >NUL

GOTO ERRORCHECK
:AFTERTEST3


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