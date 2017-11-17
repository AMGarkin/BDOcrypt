@echo off
set project=BDOcrypt

mkdir %project%\source 2>nul
mkdir %project%\bin\x64 2>nul
mkdir %project%\bin\x86 2>nul

rem ### copy source files
copy /Y License.txt %project%\source\ >nul
copy /Y Readme-Source.txt %project%\source\ >nul
copy /Y *.c* %project%\source\ >nul
copy /Y *.h %project%\source\ >nul

rem ### build executables
call build-x64.cmd
call build-x86.cmd

rem ### copy executables
copy /Y bin\x64\*.exe %project%\bin\x64\ >nul
copy /Y bin\x86\*.exe %project%\bin\x86\ >nul

rem ### copy readme
copy /Y readme.txt %project%\

rem ### create archive
"c:\Program Files\7-Zip\7z.exe" a %project%_distr.zip %project%\*
