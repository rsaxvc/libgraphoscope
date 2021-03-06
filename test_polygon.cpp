#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include "graphoscope.h"

#ifndef M_PI
	#define M_PI           3.14159265358979323846
#endif

#define SQRT2_2 0.707107

int main(int num_args, const char * const args[] )
{
size_t num_edges;
if( num_args == 1 || atoi( args[1] ) == 0 )
	{
	std::cout<<"Usage: "<<args[0]<<" number-of-edges-on-polygon"<<std::endl;
	exit(3);
	}
else
	{
	num_edges = atoi( args[1] );
	}

if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
    printf("Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
    }

gos_powerup();

gos_handle handle = gos_open();
gos_move( handle, 0.0f, 1.0f );
while(1)
	{
	gos_wait_for_buffer( handle );
	for( size_t i = 0; i <= num_edges; ++i )
		{
		float x,y;
		x = sin( (float)i * M_PI * 2 / (float)num_edges ) * .85;
		y = cos( (float)i * M_PI * 2 / (float)num_edges ) * .85;
		gos_line( handle, x, y );
		gos_line_rel( handle,  0.1f, 0.0f );
		gos_move_rel( handle, -0.1f, 0.0f );
		}
	gos_render( handle );
	}

gos_close( &handle );
gos_powerdown();
SDL_Quit();
}
