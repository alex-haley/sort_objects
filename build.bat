@echo off

IF "%1"=="" goto opt
IF "%1"=="opt" goto opt
IF "%1"=="noopt" goto noopt

:opt
cl -nologo -fp:fast -Od -Oi sort_objects_optimized.cpp -o soo.exe && soo
goto :eof

:noopt
cl -nologo -fp:fast -Od -Oi sort_objects_not_optimized.cpp -o sono.exe && sono
goto :eof
