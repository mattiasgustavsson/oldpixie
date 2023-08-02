@echo off
%~d0
cd %~p0
PixiePCT -16bitAlphaCrop -dither %*
echo..
pause