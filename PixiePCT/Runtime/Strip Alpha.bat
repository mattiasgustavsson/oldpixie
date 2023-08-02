@echo off
%~d0
cd %~p0
PixiePCT -alpha -strip %*
echo..
pause