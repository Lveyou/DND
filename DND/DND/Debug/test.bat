@echo off
echo ��������������
set count=100
echo ѭ������%count%

for /l %%i in (1,1,%count%) do (
	echo %%i%
	start DNDBird.exe
)
@ping -n 5 127.1>nul
taskkill /f /im DNDBird.exe 

pause