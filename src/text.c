#include <cairo.h>
#include <string.h>
#include "common.h"

void draw_text_line(cairo_t *cr, int width, int height, const char *text, double physical_scale) {
    if (!text || strlen(text) == 0)
        return;

    cairo_save(cr);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12 * physical_scale);
    cairo_set_source_rgb(cr, 1, 1, 1);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, text, &extents);

    double x = (width - extents.width) / 2.0 - extents.x_bearing;
    double y = height - 20 * physical_scale;

    cairo_move_to(cr, x, y);
    cairo_show_text(cr, text);

    cairo_restore(cr);
}
