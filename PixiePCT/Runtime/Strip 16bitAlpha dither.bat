@echo off
%~d0
cd %~p0
PixiePCT -16bitAlpha -strip -dither %*
echo..
pause