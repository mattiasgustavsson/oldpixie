@echo off
%~d0
cd %~p0
PixiePCT -16bit -strip -dither %*
echo..
pause