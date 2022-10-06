@echo off

IF exist ..\Build\Debug_DLL_Exe\Output ( echo Debug_DLL_Exe directory exists ) ELSE ( echo Creating Debug_DLL_Exe Dir && mkdir ..\Build\Debug_DLL_Exe\Output )
IF exist ..\Build\Release_DLL_Exe\Output ( echo Release_DLL_Exe directory exists ) ELSE ( echo Creating Release_DLL_Exe Dir && mkdir ..\Build\Release_DLL_Exe\Output )


echo Adding DLL Files...

xcopy ..\..\HawtLib\Build\Debug_DLL\Output\HawtLib.dll ..\Build\Debug_DLL_Exe\Output\ /Y
xcopy ..\..\HawtLib\Build\Release_DLL\Output\HawtLib.dll ..\Build\Release_DLL_Exe\Output\ /Y
 

echo Done!

pause