/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Deepak Meena <who53@disroot.org>
 */

#include <cairo.h>
#include <string.h>
#include "common.h"
#include "logo.h"

static cairo_status_t png_read(void *closure, unsigned char *data, unsigned int length) {
    unsigned char **p = (unsigned char **)closure;
    memcpy(data, *p, length);
    *p += length;
    return CAIRO_STATUS_SUCCESS;
}

void draw_splash(int width, int height, cairo_t *cr, double physical_scale) {
    int virtual_size = 200;
    double scale_x = (double)width / virtual_size;
    double scale_y = (double)height / virtual_size;
    double virtual_scale = scale_x < scale_y ? scale_x : scale_y;
    double final_scale = virtual_scale / physical_scale;
    double offset_x = (width - virtual_size * final_scale) / 2;
    double offset_y = (height - virtual_size * final_scale) / 2;

    cairo_set_source_rgb(cr, .118, .118, .118);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    unsigned char *data_ptr = logo;
    cairo_surface_t *image = cairo_image_surface_create_from_png_stream(png_read, &data_ptr);

    int img_width = cairo_image_surface_get_width(image);
    int img_height = cairo_image_surface_get_height(image);

    double img_scale_x = (double)virtual_size / img_width;
    double img_scale_y = (double)virtual_size / img_height;
    double img_scale = img_scale_x < img_scale_y ? img_scale_x : img_scale_y;

    cairo_save(cr);

    cairo_translate(cr, offset_x, offset_y);
    cairo_scale(cr, final_scale, final_scale);
    cairo_translate(cr, (virtual_size - img_width * img_scale) / 2.0, 
                         (virtual_size - img_height * img_scale) / 2.0);
    cairo_scale(cr, img_scale, img_scale);

    cairo_set_source_surface(cr, image, 0, 0);
    cairo_paint(cr);

    cairo_restore(cr);
    cairo_surface_destroy(image);
}
