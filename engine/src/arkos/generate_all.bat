@echo off

set Players=AKG,AKM,AKY,LW

for %%I in (%Players%) do (
	echo ==== Generate %%I ====
	call generate %%I
	echo.
)

REM pause