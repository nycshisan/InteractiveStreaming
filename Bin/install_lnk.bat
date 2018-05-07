@echo off

set shortCutPath=%cd%\..\
set shortCutName=publisher
set StartPath=%cd%
set shortCutSource=%cd%\Client.exe
echo Dim WshShell,Shortcut>>tmp.vbs
echo Dim path,path1,fso>>tmp.vbs
echo sourcepath="%shortCutSource%">>tmp.vbs
echo startpath="%StartPath%">>tmp.vbs
echo Set fso=CreateObject("Scripting.FileSystemObject")>>tmp.vbs
echo Set WshShell=WScript.CreateObject("WScript.Shell")>>tmp.vbs
echo Set Shortcut=WshShell.CreateShortCut("%shortCutPath%\%shortCutName%.lnk")>>tmp.vbs
echo Shortcut.TargetPath=sourcepath>>tmp.vbs
echo Shortcut.WorkingDirectory=startpath>>tmp.vbs
echo Shortcut.Arguments="-publisher">>tmp.vbs
echo Shortcut.Save>>tmp.vbs
"%SystemRoot%\System32\WScript.exe" tmp.vbs
del /f /q tmp.vbs

set shortCutPath=%cd%\..\
set shortCutName=player
set StartPath=%cd%
set shortCutSource=%cd%\Client.exe
echo Dim WshShell,Shortcut>>tmp.vbs
echo Dim path,path1,fso>>tmp.vbs
echo sourcepath="%shortCutSource%">>tmp.vbs
echo startpath="%StartPath%">>tmp.vbs
echo Set fso=CreateObject("Scripting.FileSystemObject")>>tmp.vbs
echo Set WshShell=WScript.CreateObject("WScript.Shell")>>tmp.vbs
echo Set Shortcut=WshShell.CreateShortCut("%shortCutPath%\%shortCutName%.lnk")>>tmp.vbs
echo Shortcut.TargetPath=sourcepath>>tmp.vbs
echo Shortcut.WorkingDirectory=startpath>>tmp.vbs
echo Shortcut.Arguments="-player">>tmp.vbs
echo Shortcut.Save>>tmp.vbs
"%SystemRoot%\System32\WScript.exe" tmp.vbs
del /f /q tmp.vbs