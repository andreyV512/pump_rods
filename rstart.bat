call "C:\Progra~2\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
msbuild pump_rods.sln /t:pump_rods /p:Configuration=Release
msbuild pump_rods.sln /t:Messages /p:Configuration=Release
pause

