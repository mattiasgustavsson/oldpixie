@echo off
%~d0
cd %~p0
PixieAGT -cpp %*
echo..
pause