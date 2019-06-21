call "C:\Progra~1\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
msbuild pump_rods.sln /t:pump_rods /p:Configuration=Debug
pause