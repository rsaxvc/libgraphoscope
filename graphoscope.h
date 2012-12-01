#ifndef GRAPHOSCOPE_H
#define GRAPHOSCOPE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

/*------------------------------------------
A litle bit about the world-space.
GOS works in a cartesian plane centered
at 0,0 and in a square with width=height=2.
This means -1,-1 is the bottom-left, and 1,1
is the top-right
------------------------------------------*/
#define GOS_MIN_X -1.0f
#define GOS_MIN_Y -1.0f
#define GOS_MAX_X  1.0f
#define GOS_MAX_Y  1.0f

/*-----------------------------------------------------------------------------
Internal handle type, none for you. Also may be different on different backends
-----------------------------------------------------------------------------*/
struct gos_type;

/*-------------------------------------
This is the handle API users get to use
-------------------------------------*/
typedef gos_type * gos_handle;

/*-------
misc goop
-------*/
void gos_powerup(void);//must be called before other GOS functions, and after SDL_init()
void gos_powerdown(void);//must be called after all other GOS functions, and before SDL_Quit()

/*--------------------------------------------------
you should probably only use one of these at a time.
Multiple soundcard support isn't done yet.
--------------------------------------------------*/
gos_handle gos_open();

/*------------------------------------------------------
This gets the current cursor position
------------------------------------------------------*/
void gos_read( gos_handle, float &x, float &y );

/*------------------------------------------------------
Apply a max and min to a given value
------------------------------------------------------*/
static __inline float gos_clip( float min, float val, float max )
{
if( val < min )
	{
	return min;
	}
if( val > max )
	{
	return max;
	}
return val;
}

/*------------------------------------------------------
Draw a line from current position to specified position.
This also sets the current position to specd position.
------------------------------------------------------*/
void gos_line( gos_handle, float x1, float y1  );

/*------------------------------------------------------
This draws a line with relative co-ordinates
------------------------------------------------------*/
static __inline void gos_line_rel( gos_handle h, float dx, float dy )
{
float x,y;
gos_read( h, x, y );
gos_line( h, gos_clip( GOS_MIN_X, x+dx, GOS_MAX_X ), gos_clip( GOS_MIN_Y, y+dy, GOS_MAX_Y ) );
}

/*------------------------------------------------------
This sets the current position to specified position.
------------------------------------------------------*/
void gos_move( gos_handle, float x1, float y1  );

/*------------------------------------------------------
This adjusts the current position by the specified amount
------------------------------------------------------*/
static __inline void gos_move_rel( gos_handle h, float dx, float dy )
{
float x;
float y;
gos_read( h, x, y );
gos_move( h, gos_clip( GOS_MIN_X, x+dx, GOS_MAX_X ), gos_clip( GOS_MIN_Y, y+dy, GOS_MAX_Y ) );
}

/*------------------------------------------------------
Compute and display the output frame. This begins looping
the current frame until another comes up.
------------------------------------------------------*/
void gos_render( gos_handle );//draws current frame

/*------------------------------------------------------
This is basically VSYNC...
------------------------------------------------------*/
void gos_wait_for_buffer( gos_handle );

/*------------------------------------------------------
Call this when done, it'll NULL out your handle too.
------------------------------------------------------*/
void gos_close( gos_handle * );

#ifdef __cplusplus
}
#endif

#endif
