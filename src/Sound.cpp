#include "Sound.h"
#include "FileManager.h"
#include "SceneManager.h"
#include "Screen.h"
#include "Logger.h"

#define BUFFER_SIZE 4096*8

Sound::Sound() : sd_loop(false), sd_pitch(1.0), sd_volume(1.0)
{
	sd_sound = new SoundData();

	if (alGetError())
		Log->Print("Error generating sound buffers.");
}

Sound::~Sound()
{
	delete sd_sound;
}

void Sound::Register()
{
	Screen* scr = Scene->GetTopScreen();
	scr->AddObject(this);
}

void Sound::Load(std::string _path)
{
	// make path local and open file
	std::string path = File->GetFile(_path);

	FILE* f;
	if (!(f = fopen(path.c_str(), "rb"))) {
		Log->Print("Could not open file \"%s\"", path.c_str());
		return;
	}

	// read ogg info
	vorbis_info *ogg_info;
	ov_open(f, &ogg_file, NULL, 0);
	ogg_info = ov_info(&ogg_file, -1);

	// work out format
	if (ogg_info->channels == 1)
		sd_sound->format = AL_FORMAT_MONO16;
	else
		sd_sound->format = AL_FORMAT_STEREO16;

	// sample rate
	sd_sound->rate = ogg_info->rate;

	Log->Print("Preparing \"%s\" (rate = %d)", _path.c_str(), sd_sound->rate);

	// position/etc
	alSource3f(sd_sound->source, AL_POSITION, 0.f, 0.f, 0.f);
	alSource3f(sd_sound->source, AL_VELOCITY, 0.f, 0.f, 0.f);
	alSource3f(sd_sound->source, AL_DIRECTION, 0.f, 0.f, 0.f);

	// distortion
	alSourcef(sd_sound->source, AL_GAIN, sd_volume);
	alSourcef(sd_sound->source, AL_PITCH, sd_pitch);

	// misc
	alSourcef(sd_sound->source, AL_ROLLOFF_FACTOR, 0.0);
	alSourcei(sd_sound->source, AL_LOOPING, sd_loop);
	alSourcei(sd_sound->source, AL_SOURCE_RELATIVE, true);

	Play();
}

bool Sound::IsPlaying()
{
	ALenum state;
	alGetSourcei(sd_sound->source, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

void Sound::Update(double delta)
{
	if (!IsPlaying())
		return;

	int processed;

	alGetSourcei(sd_sound->source, AL_BUFFERS_PROCESSED, &processed);

	while(processed--)
	{
		ALuint buffer;        
		alSourceUnqueueBuffers(sd_sound->source, 1, &buffer);

		Stream(buffer);

		alSourceQueueBuffers(sd_sound->source, 1, &buffer);
	}
}

void Sound::Play()
{
	Log->Print("Playing.");

	if (IsPlaying())
		return;

	if (!Stream(sd_sound->buffers[0]))
		return;

	if (!Stream(sd_sound->buffers[1]))
		return;

	alSourceQueueBuffers(sd_sound->source, 2, sd_sound->buffers);
	alSourcePlay(sd_sound->source);
}

/*
 * From: http://www.devmaster.net/articles/openal-tutorials/lesson8.php
 * ...it's not much different from the original.
 */
bool Sound::Stream(ALuint buffer)
{
	char data[BUFFER_SIZE];
	int size = 0;
	int section;
	int result;

	while(size < BUFFER_SIZE)
	{
		result = ov_read(&ogg_file, data + size, BUFFER_SIZE - size, 0, 2, 1, &section);

		if(result > 0)
			size += result;
		else
		{
			if(result < 0)
				Log->Print("Something is wrong!");
			else
				break;
		}
	}

	if(size == 0)
		return false;

	// do filtering around here

	alBufferData(buffer, sd_sound->format, data, size, sd_sound->rate);

	return true;
}

// Lua
#include <SLB/SLB.hpp>

void Sound_Binding()
{
	SLB::Class<Sound>("Sound")
	.constructor()
	.set("Load", &Sound::Load)
	.set("Volume", &Sound::Volume)
	.set("Pitch", &Sound::Pitch)
	.set("Loop", &Sound::Loop)
	.set("Register", &Sound::Register);
}
