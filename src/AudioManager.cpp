#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <cstring>
#include <math.h>
#include <vector>
#include "AudioManager.h"
#include "Logger.h"

ALCdevice *device;
ALCcontext *context;

std::vector<SoundData *> vpSounds;

SoundData::SoundData()
{
	alGenBuffers(2, buffers);
	alGenSources(1, &source);
	error = alGetError();
}

SoundData::~SoundData()
{
	alDeleteBuffers(2, buffers);
	alDeleteSources(1, &source);
}

void SoundData::Register()
{
	AudioManager::AddSound(this);
}

void AudioManager::AddSound(SoundData *_sound)
{
	vpSounds.push_back(_sound);
}

// finicky
void AudioManager::Clear()
{
	while (!vpSounds.empty())
	{
		delete vpSounds.back();
		vpSounds.pop_back();
	}
}

void AudioManager::Open()
{
	// open default device.
	device = alcOpenDevice(NULL);
	
	if (!device) {
		Log->Print("[Audio::Open] Failed to open default sound device.");
		return;
	}
	
	// create device context
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	// clear errors
	alGetError();
}

void AudioManager::Close()
{
	Clear();
	// clean up
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
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
	int frequency = 250;
	freq = samples * frequency;
	size = 16384; // 16k buffer
	// build the sine wave
	sineWave = new unsigned char[size];
	
	for (int i = 0; i < size; i++)
	{
		float x = i * 360.f / (float)samples;
		sineWave[i] = sinf(x * (3.14159 / 180))*128+128;
	}
	
	data = sineWave;
	delete[] sineWave;
	alBufferData(sound->buffers[0], AL_FORMAT_STEREO16, data, size, freq);
	
	if (alGetError() != AL_NO_ERROR)
		return -2;
		
	alSourcei(sound->source, AL_LOOPING, (ALboolean)true);
	alSourcei(sound->source, AL_BUFFER, sound->buffers[0]);
	alSourcef(sound->source, AL_GAIN, 0.5f); // volume
	alSourcePlay(sound->source);
	sound->Register();
	return 0;
}
