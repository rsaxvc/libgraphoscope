//c++ish
#include <cassert>
#include <cstddef>
#include <cstdlib>

//c
#include <stdint.h>

//c SDL
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_mutex.h>

//c++
#include <list>
#include <vector>

//local
#include "graphoscope.h"
#include "graphoscope_config.h"

//private
#include "pt.h"
#include "line.h"
#include "audio_buffer.h"

#define WORLD_SCALAR 30000.0f

static void play_next_buffer( int channel );

struct gos_type
	{
	std::vector<line> lines;

	//user supplied
	float cur_x;
	float cur_y;

	//rendering info
	int channel;

	audio_buffer_type * current;
	audio_buffer_type * next;

	SDL_cond  * cond;
	SDL_mutex * mutex;

	//mutex must be locked here
	bool buffer_empty(const audio_buffer_type & );
	bool buffer_full( const audio_buffer_type & );
	};

//only support one for now, later use a std::map to convert SDL channel idx to gos_type ptr
static gos_type * global_gos = NULL;

void gos_move( gos_handle h, float x, float y )
{
h->cur_x = x * WORLD_SCALAR;
h->cur_y = y * WORLD_SCALAR;
}

void gos_read( gos_handle h, float &x, float &y )
{
x = h->cur_x;
y = h->cur_y;
}

audio_buffer_type * generate_silence( size_t count )
{
assert( count > 0 );
line l={{0,0},{0,0}};
std::vector<line> lines;
for( size_t i = 0; i < count; ++i )
	{
	lines.push_back(l);
	}
return new audio_buffer_type( lines );
}

gos_handle gos_open()
{
gos_type * handle;
handle = new gos_type;

handle->cond    = SDL_CreateCond();
handle->mutex   = SDL_CreateMutex();
handle->channel = -1;
handle->cur_x   = 0;
handle->cur_y   = 0;

handle->next = NULL;
handle->current = generate_silence(100);

handle->channel = Mix_PlayChannel(handle->channel, handle->current->sound, 0);
if(handle->channel == -1)
	{
	printf("Unable to play samples: %s\n", Mix_GetError() );
	}

assert( global_gos == NULL );
global_gos = handle;


return handle;
}

void gos_line( gos_handle h, float x, float y )
{
x *= WORLD_SCALAR;
y *= WORLD_SCALAR;
line l={{x,y},{h->cur_x,h->cur_y}};
h->cur_x = x;
h->cur_y = y;
h->lines.push_back(l);
}

void gos_render( gos_handle h )
{
if( h->lines.size() == 0 )
	{
	return;
	}

audio_buffer_type * newest = new audio_buffer_type( h->lines );

h->lines.clear();

SDL_LockAudio();
//ensure nothing left in next buffer set
if( h->next != NULL )
	{
	delete( h->next );
	h->next = NULL;
	}

//copy over new buffers
h->next = newest;

SDL_UnlockAudio();

if( !Mix_Playing( h->channel ) )
	{
	//Play our sound buffer, and capture the channel on which it is played
	h->channel = Mix_PlayChannel(h->channel, h->next->sound, 0);
	if(h->channel == -1)
		{
		printf("Unable to play samples: %s\n", Mix_GetError());
		}
	}
}

void gos_close( gos_handle * deleteme )
{
assert( *deleteme == global_gos );
global_gos = NULL;

delete *deleteme;
*deleteme = NULL;
}

void gos_powerup( void )
{
//Initialize SDL_mixer with our chosen audio settings, and always use Stereo
if(Mix_OpenAudio(SAMPLE_RATE, AUDIO_S16SYS, 2, SAMPLE_BUFFER_COUNT) != 0)
	{
	printf("Unable to initialize audio: %s\n", Mix_GetError());
	exit(1);
	}

Mix_ChannelFinished( play_next_buffer );
}

void gos_wait_for_buffer( gos_handle h )
{
SDL_mutexP( h->mutex );
while( h->next != NULL )
	{
	SDL_CondWait( h->cond, h->mutex);
	}
SDL_mutexV( h->mutex );
}

static void play_next_buffer( int channel )
{
printf("filling\n");
SDL_mutexP( global_gos->mutex );
if( global_gos->next != NULL )
	{
	global_gos->next->check();

	//shift cur to last
	audio_buffer_type * last = global_gos->current;

	//shift next to cur
	global_gos->current = global_gos->next;

	//start playing cur
	global_gos->channel = Mix_PlayChannel( global_gos->channel, global_gos->current->sound, 0 );

	//invalidate next
	global_gos->next = NULL;

	//destroy last
	delete( last );
	}
else
	{
	global_gos->current->check();

	//restart playing cur
	global_gos->channel = Mix_PlayChannel( global_gos->channel, global_gos->current->sound, 0 );
	}

SDL_CondBroadcast( global_gos->cond );
SDL_mutexV( global_gos->mutex );
}

void gos_powerdown( void )
{
//Need to make sure that SDL_mixer and SDL have a chance to clean up
Mix_CloseAudio();
}
