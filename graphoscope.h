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
Draw a line from current position to specified position.
This also sets the current position to specd position.
------------------------------------------------------*/
void gos_line( gos_handle, float x1, float y1  );

/*------------------------------------------------------
This sets the current position to specified position.
------------------------------------------------------*/
void gos_move( gos_handle, float x1, float y1  );

/*------------------------------------------------------
This gets the current cursor position
------------------------------------------------------*/
void gos_read( gos_handle, float &x, float &y );

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
