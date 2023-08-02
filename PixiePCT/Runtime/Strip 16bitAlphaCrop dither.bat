@echo off
%~d0
cd %~p0
PixiePCT -16bitAlphaCrop -strip -dither %*
echo..
pause