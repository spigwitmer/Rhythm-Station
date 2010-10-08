#include "RStation.h"
#include "AudioManager.h"
#include <stdio.h>
#include "MathUtils.h"
#include <math.h>

AudioManager* Audio = NULL;

SoundData::SoundData()
{
	alGenBuffers(1, &this->buffer);
	alGenSources(1, &this->source);
	this->error = alGetError();
}

SoundData::~SoundData()
{
	alDeleteBuffers(1, &this->buffer);
	alDeleteSources(1, &this->source);
}

void SoundData::Register()
{
	Audio->AddSound(this);
}

AudioManager::AudioManager()
{
	// open default device.
	device = alcOpenDevice(NULL);
	if (!device)
	{
		Log->Print("[Audio::Open] Failed to open default sound device.");
		return;
	}

	// create device context
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	// clear errors
	alGetError();
}

AudioManager::~AudioManager()
{
	Clear();
	// clean up
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void AudioManager::AddSound(SoundData *_sound)
{
	vpSounds.push_back(_sound);
}

void AudioManager::Clear()
{
	while (!vpSounds.empty())
	{
		delete vpSounds.back();
		vpSounds.pop_back();
	}
}


// sound test.
int sine_wave()
{
	// create the buffers
	SoundData *sound = new SoundData();
	if (alGetError() != AL_NO_ERROR)
		return -1;

	ALsizei size, freq;
	ALvoid *data;

	// wave parameters and buffer size
	unsigned char *sineWave;
	int samples = 16;
	int frequency = 200;
	freq = samples * frequency;
	size = 16384; // 16k buffer
	
	// build the sine wave
	sineWave = new unsigned char[size];
	for (int i = 0; i < size; ++i)
	{
		float x = i * 360.f / (float)samples;
		sineWave[i] = sinf(radf(x))*128+128;
	}
	data = sineWave;
	delete[] sineWave;

	alBufferData(sound->buffer, AL_FORMAT_STEREO16, data, size, freq);

	if (alGetError() != AL_NO_ERROR)
		return -2;

	alSourcei(sound->source, AL_LOOPING, (ALboolean)true);
	alSourcei(sound->source, AL_BUFFER, sound->buffer);
	alSourcef(sound->source, AL_GAIN, 0.5f); // volume

	alSourcePlay(sound->source);

	sound->Register();

	return 0;
}