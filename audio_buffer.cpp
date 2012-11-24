#include <cassert>
#include <cmath>

#include <limits.h>
#include "audio_buffer.h"
#include "graphoscope_config.h"

static float dist( const pt & p1, const pt & p2 )
{
return sqrt( (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) );
}

static float CLIP( float value, int min, int max )
{
if( value < min )return min;
if( value > max )return max;
else             return value;
}

void audio_buffer_type::insert(  const pt & p )
{
buffer.push_back( CLIP( p.x, SHRT_MIN, SHRT_MAX ) );
buffer.push_back( CLIP( p.y, SHRT_MIN, SHRT_MAX ) );
}

audio_buffer_type::audio_buffer_type(const std::vector<line> & lines)
{
assert( lines.size() > 0 );

const pt * iter_pos = &lines[0].p1;

//insert the first point
insert( *iter_pos );

for( size_t i = 0; i < lines.size(); ++i )
	{
	const pt & p1=lines[i].p1;
	const pt & p2=lines[i].p2;

	if( *iter_pos != p1 )
		{
		//we have to split move commands, so add an extra position.
		//moves are also allowed to exceed slew rate
		insert( p1 );
		}

	//Add 1 to ensure we don't go above slew rate
	float angle = atan2( p2.y-p1.y, p2.x-p1.x );
	float distance = dist( p1, p2 );
	size_t projection_count =  ( distance + SLEW_RATE ) / SLEW_RATE;
	for( size_t j = 0; j <= projection_count; ++j )
		{
		pt temp=
			{
			p1.x + cosf(angle) * ( j * distance ) / projection_count,
			p1.y + sinf(angle) * ( j * distance ) / projection_count
			};
		insert( temp );
		}

	iter_pos = &lines[i].p2;
	}


sound = Mix_QuickLoad_RAW( (Uint8*)&buffer[0], buffer.size() * sizeof(int16_t) * NUM_CHANNELS );
if( sound == NULL )
	{
	printf("Unable to load raw samples: %s\n", Mix_GetError());
	}
}

audio_buffer_type::~audio_buffer_type()
{
Mix_FreeChunk( sound );
}

void audio_buffer_type::check()
{
assert(
	( buffer.size() == 0 && sound == NULL )||
	( buffer.size() != 0 && sound != NULL )||
	0 );
}
