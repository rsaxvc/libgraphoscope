//c++ish
#include <cassert>
#include <cstddef>
#include <cstdlib>

//c
#include <stdint.h>

//c SDL
#include <SDL/SDL.h>

//C GL
#include <GL/gl.h>
#include <GL/glu.h>


#define glError() glError2( __FILE__, __LINE__ )
static __inline bool glError2( const char * filename, int lineno )
{
bool retn = false;
GLenum err = glGetError();
while ( err != GL_NO_ERROR )
	{
	fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), filename, lineno );
	err = glGetError();
	retn = true;
	}
return retn;
}

//local
#include "graphoscope.h"
#include "graphoscope_config.h"

//private
#include "pt.h"
#include "line.h"
#include "audio_buffer.h"

#define DISPLAY_W 600
#define DISPLAY_H 600

static void start_draw(void);
static void finish_draw(void);

struct gos_type
	{
	//user supplied
	float cur_x;
	float cur_y;
	};

static void process_events( void )
{
/* Our SDL event placeholder. */
SDL_Event event;

/* Grab all the events off the queue. */
while( SDL_PollEvent( &event ) )
    {
    switch( event.type )
        {
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
            exit( 0 );
            break;
        }
    }
}


void gos_move( gos_handle h, float x, float y )
{
process_events();
h->cur_x = x;
h->cur_y = y;
}

void gos_read( gos_handle h, float &x, float &y )
{
x = h->cur_x;
y = h->cur_y;
}

gos_handle gos_open()
{
gos_type * handle;
handle = new gos_type;

handle->cur_x   = 0;
handle->cur_y   = 0;

start_draw();

return handle;
}

void gos_line( gos_handle h, float x, float y )
{
process_events();
#define line( _pt1,_pt2 ) glVertex3fv( _pt1 );glVertex3fv( _pt2 )
GLfloat v0[] = { h->cur_x, h->cur_y, 0.0f };
GLfloat v1[] = { (float)x, (float)y, 0.0f };
line( v0, v1 );
h->cur_x = x;
h->cur_y = y;

#undef line
}

static void finish_draw(void)
{
glEnd();
glError();

glPopMatrix();
glError();
}

static void start_draw(void)
{
/* Clear the color and depth buffers. */
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
glError();

/* We don't want to modify the projection matrix. */
glMatrixMode( GL_MODELVIEW );
glError();
glLoadIdentity( );
glError();
/* Move down the z-axis. */
glTranslatef( 0.0f, 0.0f, -2.0 ); glError();

glPushMatrix();
glError();

glBegin( GL_LINES );
}

void gos_render( gos_handle h )
{
finish_draw();

/*
 * Swap the buffers. This this tells the driver to
 * render the next frame from the contents of the
 * back-buffer, and to set all rendering operations
 * to occur on what was the front-buffer.
 */
SDL_GL_SwapBuffers( );

start_draw();
}

void gos_close( gos_handle * deleteme )
{
finish_draw();
delete *deleteme;
*deleteme = NULL;
}

void gos_powerup( void )
{
const int width = DISPLAY_W;
const int height = DISPLAY_H;
float ratio = (float) width / (float) height;

/* Information about the current video settings. */
const SDL_VideoInfo* info = NULL;

/* Color depth in bits of our window. */
int bpp = 0;

/* Flags we will pass into SDL_SetVideoMode. */
int flags = 0;

/* First, initialize SDL's video subsystem. */
if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        exit( 1 );
    }

    /* Let's get some video information. */
    info = SDL_GetVideoInfo( );

    if( !info ) {
        /* This should probably never happen. */
        fprintf( stderr, "Video query failed: %s\n",
             SDL_GetError( ) );
        exit( 2 );
    }

    bpp = info->vfmt->BitsPerPixel;

    /*
     * Now, we want to setup our requested
     * window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green
     * and blue. We also want at least a 16-bit
     * depth buffer.
     *
     * The last thing we do is request a double
     * buffered window. '1' turns on double
     * buffering, '0' turns it off.
     *
     * Note that we do not use SDL_DOUBLEBUF in
     * the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only
     * the standard 2D blitting setup.
     */

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
//VSYNC SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,0);

    /*
     * We want to request that SDL provide us
     * with an OpenGL window, in a fullscreen
     * video mode.
     *
     * EXERCISE:
     * Make starting windowed an option, and
     * handle the resize events properly with
     * glViewport.
     */
    flags = SDL_OPENGL | SDL_FULLSCREEN;
    flags = SDL_OPENGL;

    /*
     * Set the video mode
     */
    if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
        /*
         * This could happen for a variety of reasons,
         * including DISPLAY not being set, the specified
         * resolution not being available, etc.
         */
        fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
        exit( 3 );
    }


/* Our shading model--Gouraud (smooth). */
//    glShadeModel( GL_LINE_SMOOTH );

/* Culling. */
glCullFace( GL_BACK );
glFrontFace( GL_CCW );
glEnable( GL_CULL_FACE );

/* Set the clear color. */
glClearColor( 0, 0, 0, 0 );

/* Setup our viewport. */
glViewport( 0, 0, width, height );

/*
 * Change to the projection matrix and set
 * our viewing volume.
 */
glMatrixMode( GL_PROJECTION );
glLoadIdentity( );

/*
 * EXERCISE:
 * Replace this with a call to glFrustum.
 */
gluPerspective( 60.0, ratio, 1.0, 1024.0 );
}

void gos_wait_for_buffer( gos_handle h )
{
//We're assumed to always be ready to draw,
//because OpenGL will throttle for us
}

void gos_powerdown( void )
{
//Need to make sure that SDL_mixer and SDL have a chance to clean up
}
