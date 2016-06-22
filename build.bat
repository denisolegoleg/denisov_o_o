@rem set PATH="C:\CMake\bin";%PATH%
call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x64

pushd build.win64.vc2015
call cmake.exe -G"Visual Studio 14 Win64" -DOpenCV_DIR="C:/opencv.build/opencv" ..
msbuild denisov_o_o.sln /property:Configuration=Debug /m
popd