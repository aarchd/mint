#include <cairo.h>
#include <cairo-ft.h>
#include <minui.h>
#include <stdio.h>
#include <stdint.h>
#include "common.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid > 0) {
        exit(0);
    }

    setsid(); 
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2) close(fd);
}

struct minui{
    gr_surface surface;
    uint32_t width;
    uint32_t height;
};

static struct minui minui;

int main(int argc, char *argv[]) {
    if (argc < 2 || (strcmp(argv[1], "--charging") == 0 && argc < 3)) {
        // charging is broken wont know if charger has been unplugged and we need to shutdown
        // splash however do what it says
        fprintf(stderr, "Usage: %s --charging BAT_NAME | --splash\n", argv[0]);
        return 1;
    }

    gr_init(true);
    gr_color(0, 0, 0, 255);
    gr_clear();

    minui.width = gr_fb_width();
    minui.height = gr_fb_height();

    minui.surface = res_malloc_surface(minui.width * minui.height * 4);
    minui.surface->row_bytes = minui.width * 4;
    minui.surface->pixel_bytes = 4;
    minui.surface->width = minui.width;
    minui.surface->height = minui.height;

    double dpi_x = (double)minui.width * 25.4 / gr_fb_mm_width();
    double dpi_y = (double)minui.height * 25.4 / gr_fb_mm_height();
    double physical_scale = ((dpi_x + dpi_y) / 2.0) / 60;
    fprintf(stderr, "Physical scale: %f\n", physical_scale);
    
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, minui.width, minui.height);
    cairo_t *cr = cairo_create(surface);

    if (strcmp(argv[1], "--charging") == 0) {
        draw_bat(minui.width, minui.height, cr, argv[2], physical_scale);
    } else if (strcmp(argv[1], "--splash") == 0) {
        draw_splash(minui.width, minui.height, cr, physical_scale);
        daemonize();
    } else {
        fprintf(stderr, "Unknown option: %s\n", argv[1]);
        goto cleanup;
        return 1;
    }
    
    cairo_surface_flush(surface);
    unsigned char *cairo_data = cairo_image_surface_get_data(surface);

    memcpy(minui.surface->data, cairo_data, minui.width * minui.height * 4);
    gr_blit(minui.surface, 0, 0, minui.width, minui.height, 0, 0);
    gr_flip();
    
    pause();

cleanup:
    if (cr) cairo_destroy(cr);
    if (surface) cairo_surface_destroy(surface);
    if (minui.surface) {
        res_free_surface(minui.surface);
        minui.surface = NULL;
    }
    gr_fb_blank(true);
    gr_exit();
    return 0;
}
