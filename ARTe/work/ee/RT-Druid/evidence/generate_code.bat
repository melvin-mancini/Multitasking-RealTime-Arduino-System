@echo off

if %1.==. goto help
for /f "delims= tokens=1" %%c in ('dir /B /S /OD %1\plugins\org.eclipse.equinox.launcher_*.jar') do set jar_launcher=%%c

if %2.==--version. goto run_version
if %3.==. goto help
goto run


:help
echo Insufficient parameters
echo Usage: code_generation.bat ^<RT-Druid_dir^> ^<oil_file^> ^<output_dir^>
goto end


:run_version
java -jar "%jar_launcher%" -application com.eu.evidence.rtdruid.oil.standalone.writer --version
goto end

:run

shift
	
set inputs=
set outputDir=

@ECHO OFF
:Loop
IF [%1]==[] GOTO Continue
set inputs=%inputs% %outputDir%
set outputDir=%1
SHIFT
GOTO Loop
:Continue


java -jar "%jar_launcher%" -data %outputDir%/workspace -application com.eu.evidence.rtdruid.oil.standalone.writer --inputFile %inputs% --outputDir %outputDir% 


:end
