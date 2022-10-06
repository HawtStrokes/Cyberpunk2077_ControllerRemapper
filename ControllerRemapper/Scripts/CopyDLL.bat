@echo off
echo Adding DLL Files...
xcopy ..\..\External\HawtLib\HawtLib\Build\Debug_DLL\Output\HawtLib.dll ..\Build\Debug_DLL\Output\ /Y
xcopy ..\..\External\HawtLib\HawtLib\Build\Debug_DLL\Output\HawtLib.dll ..\Build\Debug_DLL_GUI\Output\ /Y

xcopy ..\..\External\HawtLib\HawtLib\Build\Release_DLL\Output\HawtLib.dll ..\Build\Release_DLL\Output\ /Y
xcopy ..\..\External\HawtLib\HawtLib\Build\Release_DLL\Output\HawtLib.dll ..\Build\Release_DLL_GUI\Output\ /Y
echo Done!
pause