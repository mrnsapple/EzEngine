mkdir  ../engine/build
cd ../engine/build
rm -rf *
cmake ..  -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target tests
./ezengine
cd ../../scripts