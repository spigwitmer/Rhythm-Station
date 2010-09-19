#include "RStation.h"
#include "Sound.h"
#include "FileManager.h"
#include "SceneManager.h"
#include "Log.h"
#include "Timer.h"

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
	#include <Vorbis/vorbisfile.h>
#elif __WINDOWS__
	#include <al/al.h>
	#include <al/alc.h>
	#include <vorbis/vorbisfile.h>
#else
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <vorbis/vorbisfile.h>
#endif

#define BUFFER_SIZE 4096

Sound::Sound()
{
	sd_sound = new SoundData();

	// create buffer to load data into
	alGenBuffers(1, &sd_sound->buffer);
	alGenSources(1, &sd_sound->source);

	ALenum err = alGetError();
	if (err)
		Log::Print("[Sound::Sound] Error generating buffers.");
	sd_waiting = false;
}

Sound::~Sound()
{
	this->deleteBuffers();
	delete sd_sound;
}

void Sound::deleteBuffers()
{
	alDeleteBuffers(1, &sd_sound->buffer);
	alDeleteSources(1, &sd_sound->source);
}

void Sound::Load(std::string _path)
{
	// make path local and open file
	std::string path = FileManager::GetFile(_path);

	FILE *f;
	if (!(f = fopen(path.c_str(), "rb")))
		return;

	// read ogg info
	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	int result = ov_open(f, &oggFile, NULL, 0);
	MessageBoxA(NULL, result == -1 ? "-1" : "not -1", "ogg", MB_OK);
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
	if ( buffer.size() > 0 )
		alBufferData(sd_sound->buffer, format, &buffer[0], static_cast<ALsizei> (buffer.size()), freq);

	sd_waiting = true;
}

void Sound::Update(float deltaTime)
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
//	alSourcef(sd_sound->source, AL_PITCH, 0.5);
	alSourcePlay(sd_sound->source);
}
