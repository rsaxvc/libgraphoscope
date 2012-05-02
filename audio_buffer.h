#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

//c SDL
#include <SDL/SDL_mixer.h>

//c++
#include <vector>

//private
#include "pt.h"
#include "line.h"

struct audio_buffer_type
	{
	Mix_Chunk * sound;
	std::vector<int16_t> buffer;
	audio_buffer_type( const std::vector<line> & lines );
	~audio_buffer_type();
	void check();
	void insert(  const pt & p );
	};

#endif
