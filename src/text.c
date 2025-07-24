/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Deepak Meena <who53@disroot.org>
 */

#define _POSIX_C_SOURCE 200809L
#include <cairo.h>
#include <cairo-ft.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "common.h"

void draw_text_line(cairo_t *cr, int width, int height, const char *text, double physical_scale) {
    if (!text || strlen(text) == 0)
        return;

    cairo_save(cr);

    FT_Library ft_library;
    FT_Face ft_face;
    FT_Init_FreeType(&ft_library);
    FT_New_Memory_Face(ft_library, font, font_len, 0, &ft_face);

    cairo_font_face_t *cairo_ft_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);
    cairo_set_font_face(cr, cairo_ft_face);
    double font_size = 14 * physical_scale;
    cairo_set_font_size(cr, font_size);
    cairo_set_source_rgb(cr, 1, 1, 1);

    double max_width = width * 0.9;

    const char *start = text;
    char *lines[128];
    int line_count = 0;

    while (*start && line_count < 128) {
        int len = 0;
        const char *last_space = NULL;

        while (start[len]) {
            if (start[len] == ' ') {
                last_space = &start[len];
            }

            char *substr = strndup(start, len + 1);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, substr, &ext);
            free(substr);

            if (ext.width > max_width) {
                if (last_space) {
                    len = last_space - start;
                }
                break;
            }
            len++;
        }

        if (len == 0)
            len = 1;

        char *line = strndup(start, len);
        lines[line_count++] = line;

        start += len;
        while (*start == ' ') start++;
    }

    double line_height = font_size * 1.2;
    double y = height - 20 * physical_scale;

    for (int i = line_count - 1; i >= 0; --i) {
        cairo_text_extents_t ext;
        cairo_text_extents(cr, lines[i], &ext);
        double x = (width - ext.width) / 2.0 - ext.x_bearing;

        cairo_move_to(cr, x, y);
        cairo_show_text(cr, lines[i]);

        y -= line_height;

        free(lines[i]);
    }

    cairo_restore(cr);
}
