#ifndef _SOUND_H_
#define _SOUND_H_

#include "Object.h"
#include "AudioManager.h"

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

// 8k is the smallest size which (consistently) works for me. Use 16k for now.
#define BUFFER_SIZE 4096*4

class Sound : public Object
{
public:
	Sound();
	virtual ~Sound();
	
	void deleteBuffers();
	void Load(std::string _path);
	void Play();
	void Update(double delta);
	
	void Loop(bool _loop) {
		sd_loop = _loop;
	}
	void Volume(float _volume) {
		sd_volume = _volume;
	}
	void Pitch(float _pitch) {
		sd_pitch = _pitch;
	}
	void Pan(float _pan) {
		sd_pan = _pan;
	}
	void UseEqualizer(bool enabled);
	
private:
	void Register();
	
	SoundData *sd_sound;
	OggVorbis_File ogg_file;
	
	bool Stream(ALuint buffer);
	bool IsPlaying();
	
	bool m_use_eq, sd_loop, sd_waiting, loaded;
	float sd_pan, sd_pitch, sd_volume;
	
	double *in;
	fftw_complex *out;
};

void Sound_Binding();

#endif
