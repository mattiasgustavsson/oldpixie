@echo off
%~d0
cd %~p0
PixiePCT -alphags -strip %*
echo..
pause