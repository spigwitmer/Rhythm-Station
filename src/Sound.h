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

class Sound : public Object
{
public:
	Sound();
	virtual ~Sound();
	void Register();

	void deleteBuffers();
	void Load(std::string _path);
	void Play();
	void Update(double delta);

	void Loop(bool _loop) { sd_loop = _loop; }
	void Volume(float _volume) { sd_volume = _volume; }
	void Pitch(float _pitch) { sd_pitch = _pitch; }
	void Pan(float _pan) { sd_pan = _pan; }

private:
	SoundData* sd_sound;
	OggVorbis_File ogg_file;

	bool Stream(ALuint buffer);
	bool IsPlaying();

	bool sd_loop, sd_waiting;
	float sd_pan, sd_pitch, sd_volume;
};

void Sound_Binding();

#endif
