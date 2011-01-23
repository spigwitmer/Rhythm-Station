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
	alGenBuffers(4, buffers);
	alGenSources(1, &source);
	error = alGetError();
}

SoundData::~SoundData()
{
	alDeleteBuffers(4, buffers);
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
