@echo off

set Project=%~f1

if not exist out ( md out )
if not exist bin ( md bin )

g++ -o bin/%Project% -Wall -Wextra -pedantic %1

pause