#ifndef GRAPHOSCOPE_H
#define GRAPHOSCOPE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef void * gos_handle;
gos_handle gos_open( int width, int height, double max_x, double max_y, double fps, double samplerate );
void gos_draw_line( gos_handle, int x1, int y1, int x2, int y2 );
void gos_draw_circle( gos_handle, int x, int y, int r );

typedef void (*gos_callback_func)( double * gos_samples_left, double * gos_samples_right, size_t count );
void gos_render( gos_callback_func );
void gos_close( gos_handle * );

#ifdef __cplusplus
}
#endif

#endif
