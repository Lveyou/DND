@echo off
echo 服务器并发测试
set count=100
echo 循环次数%count%

for /l %%i in (1,1,%count%) do (
	echo %%i%
	start DNDBird.exe
)
@ping -n 5 127.1>nul
taskkill /f /im DNDBird.exe 

pause