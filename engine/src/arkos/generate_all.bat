@echo off

set Players=akg,akm,aky,lw

for %%I in (%Players%) do (
	echo ========================================
	echo Generate %%I
	call generate %%I
)

pause