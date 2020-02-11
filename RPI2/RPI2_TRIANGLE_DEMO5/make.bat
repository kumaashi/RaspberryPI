@echo off
makesub.bat 2>&1 | tee res.txt & grep erro res.txt
REM add.bat

