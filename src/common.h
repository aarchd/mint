#ifndef COMMON_H
#define COMMON_H

#include <cairo.h>

void draw_bat(int width, int height, cairo_t *cr, char *batname, double physical_scale);

void draw_splash(int width, int height, cairo_t *cr, double physical_scale);

#endif // COMMON_H
