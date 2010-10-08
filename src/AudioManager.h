#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <string>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

struct SoundData
{
	SoundData();
	virtual ~SoundData();
	void Register();
	
	// buffer and source data, error handle (just in case)
	ALuint buffer;
	ALuint source;
	ALenum error;
	
	// audio file path
	std::string path;
};

int sine_wave();

class AudioManager
{
public:
	AudioManager();
	virtual ~AudioManager();

	void AddSound(SoundData *sound);
	void Clear();

private:
	ALCdevice* device;
	ALCcontext* context;

	std::vector<SoundData*> vpSounds;
};

extern AudioManager* Audio;

#endif
