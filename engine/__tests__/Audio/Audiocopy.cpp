/** 
 * @author Oriol Manzano
 */

#include <gtest/gtest.h>
#include "SceneManager.hpp"
#include "SceneComponent.hpp"
#include <memory>
#include "Scene.hpp"
#include "AudioComponent.hpp"
#include "Collision.hpp"

#include "Stream/AudioStreamOutput.hpp"

#include <fstream>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>
#include "ResourceManager.hpp"
#include "ThreadPoolInternal.hpp"
#include <stdlib.h> 

#include <filesystem>
#include <iostream>
#include <portaudio.h>
#include <sndfile.h>

#include "Initialization/Initialization.hpp"

using namespace ez;
namespace fs = std::filesystem;

struct f
{
	int chunckid;
	int chuncksize;
	int chunckformat;

	int subchunckid1;
	int Subchunk1Size;
	short AudioFormat;
	short NumChannels;
	int SampleRate;
	int ByteRate;
	short BlockAlign;
	short BitsPerSample;

	int Subchunk2ID;
	int Subchunk2Size;
};





SF_INFO	info;
std::string path="";
SNDFILE *filee;
typedef struct
{
	short left_phase;
	short right_phase;
}	paTestData;

static int patestCallback(  const void *inputBuffer, void *outputBuffer,
						   unsigned long framesPerBuffer,
						   const PaStreamCallbackTimeInfo* timeInfo,
						   PaStreamCallbackFlags statusFlags,
						   void *userData ) {
	//printf("%d\n", (int) framesPerBuffer);
	//std::cout << "frameperbuffer"<<std::endl<<std::endl<<std::endl;

	/* Cast data passed through stream to our structure. */
	paTestData *data = (paTestData *) outputBuffer;
	

	auto num_read = sf_read_short(filee, (short *) outputBuffer, framesPerBuffer * info.channels);
/*
	file.read((char *) &data->left_phase, sizeof(short));
	file.read((char *) &data->right_phase, sizeof(short));
	std::memcpy(outputBuffer, data, sizeof(paTestData));
	if (file.eof()) {
		std::cout << "complete" << std::endl;
		return paComplete;
	}*/
	if (num_read < framesPerBuffer)
		return paComplete;
	return paContinue;
}

TEST(SceneTest, SceneCompoludnentsChilds) {
	 path = fs::current_path().parent_path().parent_path().parent_path().u8string();
	path += "\\engine\\__tests__\\Audio\\LRMonoPhase4.wav";

	std::ifstream file(path);

	std::cout << "Path:" << path;
	filee = sf_open(path.c_str(), SFM_READ, &info);

	Initialization	init;
	f _;
	std::cout << sizeof(f) << std::endl;
	if (!file)
		EXPECT_EQ(1, 2);
	else
		EXPECT_EQ(1, 1);

	file.read((char *) &_, sizeof(f));
	std::cout << std::hex << _.chunckid << std::endl;
	std::cout << std::hex << _.chuncksize << std::endl;
	std::cout << std::hex << _.chunckformat << std::endl;

	std::cout << std::hex << _.subchunckid1 << std::endl;
	std::cout << std::hex << _.Subchunk2ID << std::endl;

	std::cout << std::hex << _.BitsPerSample << std::endl;
	std::cout << std::dec << _.NumChannels<< std::endl;
	std::cout << std::dec << _.BitsPerSample << std::endl;
	std::cout << std::dec << _.NumChannels * _.BitsPerSample<< std::endl;
	std::cout << std::dec << _.BlockAlign << std::endl;
	std::cout << "sample rate: " << _.SampleRate << std::endl;

	std::cout << "info rate: " << info.samplerate << std::endl;

	ez::AudioStreamOutput	output(patestCallback,  info.samplerate, _.BlockAlign, details::SampleFormat::Int16);
	std::cout << "before play"<<std::endl;

	output.play();
	std::cout << "after play"<<std::endl;

	while (output.isActive()) {
		std::cout << "Loop"<<std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(15));
	}

	
	EXPECT_EQ(1, 1);

}
