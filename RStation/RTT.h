#ifndef _RTT_H_
#define _RTT_H_

#include "RStation.h"

void create_fbo();
void delete_fbo();
void draw_fbo();

GLuint get_framebuffer();
GLuint get_framebuffer_tex();

#endif