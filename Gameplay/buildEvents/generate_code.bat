pushd "%~dp0" 
set current_dir=%cd%
@REM py.exe %current_dir%\run_code_generation.py
%current_dir%\run_code_generation.exe
popd