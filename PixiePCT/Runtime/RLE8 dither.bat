@echo off
%~d0
cd %~p0
PixiePCT -rle8 -dither %*
echo..
pause