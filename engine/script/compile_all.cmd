@echo off

for %%G in (%SrcList%) do (
	call %LibDir%\script\compile.cmd %%G
    if errorlevel 1 (exit /B 300)
)

exit /B 0