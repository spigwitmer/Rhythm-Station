#include "Sound.h"
#include "FileManager.h"
#include "SceneManager.h"
#include "Screen.h"
#include "Logger.h"

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
	#include <Vorbis/vorbisfile.h>
#else
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <vorbis/vorbisfile.h>
#endif

#define BUFFER_SIZE 4096

Sound::Sound() : sd_loop(false), sd_pitch(1.0), sd_volume(1.0)
{
	sd_sound = new SoundData();

	// create buffer to load data into
	alGenBuffers(1, &sd_sound->buffer);
	alGenSources(1, &sd_sound->source);

	ALenum err = alGetError();
	if (err)
		Log->Print("[Sound::Sound] Error generating buffers.");
	sd_waiting = false;
}

Sound::~Sound()
{
	this->deleteBuffers();
	delete sd_sound;
}

void Sound::Register()
{
	Screen *scr = Scene->GetTopScreen();
	scr->AddObject(this);
}

void Sound::deleteBuffers()
{
	alDeleteBuffers(1, &sd_sound->buffer);
	alDeleteSources(1, &sd_sound->source);
}

// limited!
void Sound::Load(std::string _path)
{
	// make path local and open file
	std::string path = File->GetFile(_path);

	FILE *f;
	if (!(f = fopen(path.c_str(), "rb")))
		return;

	// read ogg info
	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	ov_open(f, &oggFile, NULL, 0);
	pInfo = ov_info(&oggFile, -1);

	// work out format
	ALenum format;
	if (pInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;

	// sample rate
	ALsizei freq = pInfo->rate;

	// sound buffer
	char array[BUFFER_SIZE];
	std::vector <char> buffer;

	// read ogg data in chunks of BUFFER_SIZE
	long bytes = 1;
	while (bytes > 0)
	{
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, 0 /* little endian */, 2, 1, NULL);
		buffer.insert(buffer.end(), array, array + bytes);
	}
	ov_clear(&oggFile);

	// upload buffer data
	if (!buffer.empty())
		alBufferData(sd_sound->buffer, format, &buffer[0], static_cast<ALsizei> (buffer.size()), freq);

	sd_waiting = true;
}

void Sound::Update(double delta)
{
	ALenum state;
	alGetSourcei(sd_sound->source, AL_SOURCE_STATE, &state);

	if (state == AL_STOPPED)
		return;

	if (sd_waiting)
	{
		this->Play();
		sd_waiting = false;
	}
}

void Sound::Play()
{
	// Attach sound buffer to source & play it
	alSourcei(sd_sound->source, AL_BUFFER, sd_sound->buffer);
	alSourcei(sd_sound->source, AL_LOOPING, sd_loop);
	alSourcef(sd_sound->source, AL_GAIN, sd_volume);
	alSourcef(sd_sound->source, AL_PITCH, sd_pitch);
	alSourcePlay(sd_sound->source);
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
