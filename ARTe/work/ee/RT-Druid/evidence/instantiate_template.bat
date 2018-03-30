@echo off

if %1.==. goto help
for /f "delims= tokens=1" %%c in ('dir /B /S /OD %1\plugins\org.eclipse.equinox.launcher_*.jar') do set jar_launcher=%%c

if %2.==--version. goto run_version
if %3.==. goto help
goto run


:help
echo Insufficient parameters
echo Usage: templates.sh ^<RT-Druid_dir^> ^<template_id^> ^<output_dir^>
goto end


:run_version
java -jar "%jar_launcher%" -application com.eu.evidence.rtdruid.oil.standalone.templates --version
goto end

:run
java -jar "%jar_launcher%" -data %3/workspace -application com.eu.evidence.rtdruid.oil.standalone.templates --templateId %2 --outputDir %3



:end
