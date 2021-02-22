mkdir  "../engine/build"
cd "../engine/build"
RMDIR . /S /Q
conan install .. --build=missing 
cmake .. 
cmake --build . 
cd bin
ezengine
cd ../../scripts
cd ../../../scripts