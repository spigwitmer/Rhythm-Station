#include "Sound.h"
#include "managers/GameManager.h"
#include "managers/FileManager.h"
#include "utils/Logger.h"

Sound::Sound() : m_use_eq(false), sd_loop(false), loaded(false), waiting(false), sd_pitch(1.0), sd_volume(1.0)
{
	sd_sound = new SoundData();
	
	if (alGetError())
		Log->Print("Error generating sound buffers.");
		
	in = new double[BUFFER_SIZE];
	out = new fftw_complex[BUFFER_SIZE];
	
	Game->AddSound(this);
}

Sound::~Sound()
{
	if (loaded)
		ov_clear(&ogg_file);

	delete sd_sound;
	
	// fft stuff
	fftw_cleanup();
	delete[] in;
	delete[] out;
}

void Sound::Load(std::string _path)
{
	// make path local and open file
	std::string path = FileManager::GetFile(_path);
	FILE *f;
	
	if (!(f = fopen(path.c_str(), "rb")))
	{
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
	
	loaded = waiting = true;
}

bool Sound::IsPlaying()
{
	ALenum state;
	alGetSourcei(sd_sound->source, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

void Sound::Update(double delta)
{
	if (waiting)
	{
		Play();
		waiting = false;
	}
	
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

	if (!Stream(sd_sound->buffers[0]) ||
		!Stream(sd_sound->buffers[1]) ||
		!Stream(sd_sound->buffers[2]) ||
		!Stream(sd_sound->buffers[3]) ||
		!Stream(sd_sound->buffers[4]) ||
		!Stream(sd_sound->buffers[5]) ||
		!Stream(sd_sound->buffers[6]) ||
		!Stream(sd_sound->buffers[7]))
		return;
	
	alSourceQueueBuffers(sd_sound->source, 8, sd_sound->buffers);
	alSourcePlay(sd_sound->source);
}

/*
 * From: http://www.devmaster.net/articles/openal-tutorials/lesson8.php
 * ...it's not much different from the original.
 *
 * Note: This is where it shows that things really should be threaded here.
 */
bool Sound::Stream(ALuint buffer)
{
	char data[BUFFER_SIZE];
	int size = 0;
	int section;
	int result;
	
	while(size < BUFFER_SIZE)
	{
		result = ov_read(&ogg_file, &data[size], BUFFER_SIZE - size, 0, 2, 1, &section);
		
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
		
	if (m_use_eq)
	{
		for (int i = 0; i<size; i++)
			in[i] = data[i];
			
		fftw_plan p = fftw_plan_dft_r2c_1d(size, in, out, FFTW_ESTIMATE);
		fftw_execute(p);
		fftw_destroy_plan(p);
		
		int bands = 20;
		int skip = size/2/bands;
		
		for (int i = 0; i<bands; i++)
		{
			// shit just got real ( http://xkcd.com/849/ )
			in[i] = sqrt(pow(out[i*skip][0],2)+pow(out[i*skip][1],2));
			
			// normalize
			in[i] /= sd_sound->rate*0.5;
		}
		
		memcpy(Game->CurrentEqualizerFrame, in, sizeof(double) * bands);
	}
	
	alBufferData(buffer, sd_sound->format, data, size, sd_sound->rate);
	return true;
}

void Sound::setLoop(bool _loop)
{
	sd_loop = _loop;
}

void Sound::setVolume(float _volume)
{
	sd_volume = _volume;
}

void Sound::setPitch(float _pitch)
{
	sd_pitch = _pitch;
}

void Sound::setPan(float _pan)
{
	sd_pan = _pan;
}

void Sound::UseEqualizer(bool enabled)
{
	m_use_eq = enabled;
}

// Lua
#include <SLB/SLB.hpp>

void Sound_Binding()
{
	SLB::Class<Sound>("Sound")
	.constructor()
	.set("UseEqualizer", &Sound::UseEqualizer)
	.set("Load", &Sound::Load)
	.set("setVolume", &Sound::setVolume)
	.set("setPitch", &Sound::setPitch)
	.set("setLoop", &Sound::setLoop)
	.set("Update", &Sound::Update)
	
	// Yeah, I know sounds don't draw.
	.set("Draw", &Sound::Draw);
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
