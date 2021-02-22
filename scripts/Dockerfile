From debian:buster-slim

RUN apt-get update && apt-get upgrade -yqq && \
    apt-get install -yqq cmake git g++ python3 python3-pip sudo > /dev/null && \
    pip3 install conan -q && \
    mkdir -p /home/project
	
RUN conan profile new --detect default && conan profile update settings.compiler.libcxx=libstdc++11 default && \
    conan profile new --detect debug && conan profile update settings.compiler.libcxx=libstdc++11 debug && \
	conan profile update settings.build_type=Debug debug && \
	conan remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"

WORKDIR /home/project
