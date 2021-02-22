mkdir  "../engine/build"
cd "../engine/build"
RMDIR . /S /Q
conan install .. --build=missing 
cmake ..  -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target tests
cd bin
tests
cd ../../../scripts