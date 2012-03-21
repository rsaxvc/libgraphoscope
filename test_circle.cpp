#include "graphoscope.h"

#define WIDTH 100
#define HEIGHT 100
#define MAX_X 1.0
#define MAX_Y 1.0
#define FPS 1.0

#define SAMPLERATE 10000

int main()
{
gos_handle handle = gos_open( WIDTH, HEIGHT, MAX_X, MAX_Y, FPS, SAMPLERATE );
gos_draw_circle( handle, 0, 0, 25 );
gos_close( &handle );
}
