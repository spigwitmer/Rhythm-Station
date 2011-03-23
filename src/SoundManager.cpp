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
#include "SoundManager.h"
#include "Logger.h"

ALCdevice *device;
ALCcontext *context;

std::vector<SoundData *> vpSounds;

SoundData::SoundData()
{
	alGenBuffers(8, buffers);
	alGenSources(1, &source);
	error = alGetError();
}

SoundData::~SoundData()
{
	alDeleteBuffers(8, buffers);
	alDeleteSources(1, &source);
}

void SoundData::Register()
{
	SoundManager::AddSound(this);
}

void SoundManager::AddSound(SoundData *_sound)
{
	vpSounds.push_back(_sound);
}

// finicky
void SoundManager::Clear()
{
	while (!vpSounds.empty())
	{
		delete vpSounds.back();
		vpSounds.pop_back();
	}
}

void SoundManager::Open()
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

void SoundManager::Close()
{
	Clear();
	
	// clean up
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
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
