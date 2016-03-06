#!/bin/sh

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]
	then
		sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
	  	wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -
	  	sudo add-apt-repository -y 'deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty main'
	  	sudo add-apt-repository -y 'deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.6 main'
	  	sudo add-apt-repository -y 'deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.7 main'
	  	sudo apt-get update -qq
	  	wget https://github.com/Viq111/travis-container-packets/releases/download/cmake-3.1.2/cmake.tar.bz2
	  	tar -xjf cmake.tar.bz2
	  	rm cmake.tar.bz2
	  	export PATH=$(pwd)/cmake/bin:$PATH
	  	sudo apt-get install -y  libtag1-dev libsndfile1-dev libportaudio2 portaudio19-dev libstdc++6-4.7-dev
elif [[ "${TRAVIS_OS_NAME}" == "osx" ]]
	 then
	 	brew update
	 	brew install portaudio taglib libsndfile
	 	
fi