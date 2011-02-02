#ifndef _SOUND_H_
#define _SOUND_H_

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <Vorbis/vorbisfile.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#endif

#include <fftw3.h>

#include "Object.h"
#include "SoundManager.h"

// 8k is the smallest size which (consistently) works for me. Use 32k for now.
#define BUFFER_SIZE 4096*8

class Sound : public Object
{
public:
	Sound();
	virtual ~Sound();
	
	void deleteBuffers();
	void Load(std::string _path);
	void Play();
	void Update(double delta);
	
	void setLoop(bool _loop);
	void setVolume(float _volume);
	void setPitch(float _pitch);
	void setPan(float _pan);
	
	void UseEqualizer(bool enabled);
	
private:	
	SoundData *sd_sound;
	OggVorbis_File ogg_file;
	
	bool Stream(ALuint buffer);
	bool IsPlaying();
	
	bool m_use_eq, sd_loop, sd_waiting, loaded, waiting;
	float sd_pan, sd_pitch, sd_volume;
	
	double *in;
	fftw_complex *out;
};

void Sound_Binding();

#endif
