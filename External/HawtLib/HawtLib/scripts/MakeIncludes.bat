@echo off

IF exist ..\Build\ ( echo Include directory exists ) ELSE ( echo Creating Include Dir && mkdir ..\Build\Includes\HawtLib\ )

echo Removing Old Files...

del /F /Q /S ..\Build\Includes\

echo Adding Files...

xcopy ..\Source\. ..\Build\Includes\HawtLib\ /E /Y
cd ..\Build\Includes\HawtLib\
for /f %%F in ('dir /s /b /a-d ^| findstr /vile ".h"') do del "%%F"

echo Deleting Non-Header Files...

echo Done!

pause; 