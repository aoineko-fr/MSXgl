@echo off
REM Generate the given Arkos replayer(s) (ex: generate aky_6ch)
"%~dp0..\..\..\tools\build\Node\node.exe" "%~dp0generate.js" %*
