#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <string>

#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif

struct SoundData
{
	SoundData();
	virtual ~SoundData();
	void Register();
	
	// buffer and source data, error handle (just in case)
	ALenum error;
	ALuint buffers[2];
	ALuint source;

	// sampling rate and channels
	ALsizei rate;
	ALenum format;

	// audio file path
	std::string path;
};

int sine_wave();

class AudioManager
{
public:
	void AddSound(SoundData *sound);
	void Clear();
	void Open();
	void Close();
};

extern AudioManager* Audio;

#endif
