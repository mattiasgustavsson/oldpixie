@echo off
%~d0
cd %~p0
PixiePCT -rle16 -strip %*
echo..
pause