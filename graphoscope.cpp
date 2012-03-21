#include <vector>

#include "graphoscope.h"

using std::vector;

struct gos_line
	{
	int x1;
	int y1;
	int x2;
	int y2;
	};

struct gos_circle
	{
	int x;
	int y;
	int r;
	};

struct gos_type
	{
	vector<gos_line> lines;
	vector<gos_circle> circles;
	};

typedef void * gos_handle;

gos_handle gos_open( int width, int height, double max_x, double max_y, double fps, double samplerate )
{
gos_type * handle;
handle = new gos_type;
return handle;
}

void gos_draw_line( gos_handle h, int x1, int y1, int x2, int y2 )
{
gos_type * handle;
handle = (gos_type*)h;
gos_line line={x1,y1,x2,y2};

handle->lines.push_back(line);
}

void gos_draw_circle( gos_handle h, int x, int y, int r )
{
gos_type * handle;
handle = (gos_type*)h;
gos_circle circle={x,y,r};

handle->circles.push_back(circle);
}

typedef void (*gos_callback_func)( double * gos_samples_left, double * gos_samples_right, size_t count );
void gos_render( gos_callback_func );

void gos_close( gos_handle * deleteme )
{
gos_type * handle = (gos_type*)(*deleteme);
delete handle;
*deleteme = NULL;
}
