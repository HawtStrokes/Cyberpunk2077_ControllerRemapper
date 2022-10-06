@echo off

IF exist ..\Build\ ( echo Include directory exists ) ELSE ( echo Creating Include Dir && mkdir ..\Build\includes\HawtLib\ )

echo Removing Old Files...

del /F /Q /S ..\Build\includes\

echo Adding Files...

xcopy ..\HawtLib\Source\. ..\Build\includes\HawtLib\ /E /Y
cd ..\Build\includes\HawtLib\
for /f %%F in ('dir /s /b /a-d ^| findstr /vile ".h"') do del "%%F"

echo Deleting Non-Header Files...

echo Done!

pause; 