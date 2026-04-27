@echo off

set Players=AKG,AKM,AKY,AKY_6ch,AKY_Darky

for %%I in (%Players%) do (
	echo ==== Generate %%I ====
	call generate %%I
	echo.
)

REM pause