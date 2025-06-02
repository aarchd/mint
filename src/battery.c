/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Deepak Meena <notwho53@gmail.com>
 */

#include <cairo.h>
#include <cairo-ft.h>
#include "common.h"

static void cairo_rounded_rectangle(cairo_t *cr, double x, double y, double width, double height, double radius) {
    double x0 = x, y0 = y;
    double x1 = x + width;
    double y1 = y + height;

    cairo_move_to(cr, x0 + radius, y0);
    cairo_line_to(cr, x1 - radius, y0);
    cairo_arc(cr, x1 - radius, y0 + radius, radius, -3.14/2, 0);
    cairo_line_to(cr, x1, y1 - radius);
    cairo_arc(cr, x1 - radius, y1 - radius, radius, 0, 3.14/2);
    cairo_line_to(cr, x0 + radius, y1);
    cairo_arc(cr, x0 + radius, y1 - radius, radius, 3.14/2, 3.14);
    cairo_line_to(cr, x0, y0 + radius);
    cairo_arc(cr, x0 + radius, y0 + radius, radius, 3.14, 3 * 3.14 / 2);
    cairo_close_path(cr);
}

static int get_battery_percentage(char *batname) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/class/power_supply/%s/capacity", batname);
    FILE *fp = fopen(path, "r");
    if (fp) {
        int percentage;
        fscanf(fp, "%d", &percentage);
        fclose(fp);
        return percentage;
    } else {
        return 0;
    }
}

void draw_bat(int width, int height, cairo_t *cr, char *batname, double physical_scale) {
    float bat_percent = get_battery_percentage(batname) / 100.0;

    double virtual_size = 200.0;
    double scale_x = (double)width / virtual_size;
    double scale_y = (double)height / virtual_size;
    double logical_scale = scale_x < scale_y ? scale_x : scale_y;

    double final_scale = logical_scale / physical_scale;
    double offset_x = (width - virtual_size * final_scale) / 2.0;
    double offset_y = (height - virtual_size * final_scale) / 2.0;

    cairo_save(cr);
    cairo_translate(cr, offset_x, offset_y);
    cairo_scale(cr, final_scale, final_scale);

    float batbox_width = 160;
    float batbox_height = 36;
    float corner_radius = batbox_height * 0.2;
    float bat_x = (virtual_size - batbox_width)/2;
    float bat_y = (virtual_size - batbox_height)/2;
    float bat_inner_width = bat_percent * batbox_width;

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_rounded_rectangle(cr, bat_x, bat_y, batbox_width, batbox_height, corner_radius);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rounded_rectangle(cr, bat_x, bat_y, batbox_width, batbox_height, corner_radius);
    cairo_clip(cr);
    
    cairo_rectangle(cr, bat_x, bat_y, bat_inner_width, batbox_height);
    cairo_fill(cr);
    cairo_reset_clip(cr);

    if (bat_percent < 0.2) {
        cairo_set_source_rgb(cr, 0.85, 0.1, 0.1);
    } else if (bat_percent < 0.5) {
        cairo_set_source_rgb(cr, 1.0, 0.55, 0.0);
    } else {
        cairo_set_source_rgb(cr, 0.2, 0.8, 0.2);
    }

    cairo_select_font_face(cr, "Poppins Light", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 16);

    char percentage_text[10];
    snprintf(percentage_text, sizeof(percentage_text), "%.0f%%", bat_percent * 100);
    cairo_text_extents_t extents;
    cairo_text_extents(cr, percentage_text, &extents);

    cairo_move_to(cr,
        (virtual_size - extents.width) / 2,
        (virtual_size + extents.height) / 2
    );
    cairo_show_text(cr, percentage_text);

    cairo_restore(cr);
}
