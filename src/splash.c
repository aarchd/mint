/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Deepak Meena <notwho53@gmail.com>
 */

#include <cairo.h>
#include "common.h"

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

    cairo_save(cr);
    
    cairo_translate(cr, offset_x, offset_y);
    cairo_scale(cr, final_scale, final_scale);
    
    cairo_move_to(cr, 100, 60);      
    cairo_line_to(cr, 170, 110);     
    
    double c1x = 170 + 2.0/3.0*(100 - 170);  
    double c1y = 110 + 2.0/3.0*(160 - 110);
    double c2x = 30 + 2.0/3.0*(100 - 30);    
    double c2y = 110 + 2.0/3.0*(160 - 110);
    cairo_curve_to(cr, c1x, c1y, c2x, c2y, 30, 110);
    cairo_close_path(cr);
    cairo_set_source_rgb(cr, 0.898, 0.224, 0.208);
    cairo_fill(cr);
    
    cairo_move_to(cr, 100, 50);
    cairo_line_to(cr, 170, 100);
    c1x = 170 + 2.0/3.0*(100 - 170);
    c1y = 100 + 2.0/3.0*(150 - 100);
    c2x = 30 + 2.0/3.0*(100 - 30);
    c2y = 100 + 2.0/3.0*(150 - 100);
    cairo_curve_to(cr, c1x, c1y, c2x, c2y, 30, 100);
    cairo_close_path(cr);
    cairo_set_source_rgb(cr, 0.937, 0.325, 0.314);
    cairo_fill(cr);
    
    cairo_move_to(cr, 100, 40);
    cairo_line_to(cr, 170, 90);
    c1x = 170 + 2.0/3.0*(100 - 170);
    c1y = 90 + 2.0/3.0*(140 - 90);
    c2x = 30 + 2.0/3.0*(100 - 30);
    c2y = 90 + 2.0/3.0*(140 - 90);
    cairo_curve_to(cr, c1x, c1y, c2x, c2y, 30, 90);
    cairo_close_path(cr);
    cairo_set_source_rgb(cr, 0.898, 0.451, 0.451);
    cairo_fill(cr);
    
    cairo_restore(cr);
}
