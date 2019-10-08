@echo off
cd /D "%~dp0"

echo Generating Visual Studio 2019 solution...


c:\premake\premake5.exe vs2019 --projectdir=D:\Workspace\CMakeProjects\Squid\Sandbox --file=D:\Workspace\CMakeProjects\Squid\Squid\premake5.lua