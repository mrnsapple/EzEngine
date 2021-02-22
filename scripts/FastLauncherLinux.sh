cd ../engine/build
cmake ..  -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target tests
./__tests__/tests
cd ../../scripts