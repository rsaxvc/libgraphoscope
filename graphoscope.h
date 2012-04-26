#ifndef GRAPHOSCOPE_H
#define GRAPHOSCOPE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

struct gos_type;

typedef gos_type * gos_handle;

void gos_powerup(void);//must be called before other GOS functions, and after SDL_init()
void gos_powerdown(void);//must be called after all other GOS functions, and before SDL_Quit()

gos_handle gos_open();
void gos_line( gos_handle, double x1, double y1  );
void gos_move( gos_handle, double x1, double y1  );
void gos_render( gos_handle );//draws current frame
void gos_wait_for_buffer( gos_handle );

void gos_close( gos_handle * );

#ifdef __cplusplus
}
#endif

#endif
