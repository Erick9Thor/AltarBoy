pushd "%~dp0" 
set current_dir=%cd%
py.exe %current_dir%\run_code_generation.py
py.exe %current_dir%\run_auto_dll_versioning.py
popd