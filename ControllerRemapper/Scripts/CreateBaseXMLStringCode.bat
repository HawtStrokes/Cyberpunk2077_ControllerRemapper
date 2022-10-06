@echo off
echo creating strBaseIC
cd MakeStringMC/x64/
makestringmc.exe ../../../Resource/BaseIC.xml ../../../Resource/strBaseIC.txt
echo Done!
echo creating strBaseUM
makestringmc.exe ../../../Resource/BaseUM.xml ../../../Resource/strBaseUM.txt
echo Done!
pause
