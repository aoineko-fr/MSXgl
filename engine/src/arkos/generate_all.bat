@echo off

set Players=akg,akm,aky,lw

for %%I in (%Players%) do (
	echo ==== Generate %%I ====
	generate %%I
)