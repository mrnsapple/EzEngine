[![pipeline status](https://gitlab.com/4year/ezengine/badges/master/pipeline.svg)](https://gitlab.com/4year/ezengine/-/commits/master) [![pipeline status](https://gitlab.com/4year/ezengine/badges/dev/pipeline.svg)](https://gitlab.com/4year/ezengine/-/commits/dev)

# Welcome to the EzEngine project

A 3D engine 


## Tests

All tests must be done inside engine/__tests __ folder.

The test job in gitlab does __not build__ the engine, therefore you have to provide all necessary libs in the
CMakefile.txt found in  engine/__tests __. To launch the test, simply launch tests or tests.exe in your build.

All tests use the google suite tests available here : https://github.com/google/googletest

## The Dockerfile

The dockerfile provided into scripts is used in conjunction of .gitlab-ci.yml, to create and deploy a docker image, design
to accelerate the testing process, without the need to reinstall each time the same packets. 

The Docker image job is done by two jobs : EnsureContainer and BuildContainer

In case of problem, please make the modification into feature/cicd.

## Building

You will need conan to install all the libraries
You will need to manually add the vulkan libraries from the 1.2.148.0 sdk version into your build folder