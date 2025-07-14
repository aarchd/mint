/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Deepak Meena <who53@disroot.org>
 *
 * minui rendering code is based on work by:
 *   Eugenio Paolantonio (g7) <me@medesimo.eu>
 *   https://github.com/g7/lv_drivers/blob/8cd6926996779d1c6a2b2e83405cf59809d8db02/display/minui.c
 *
 */

#include <cairo.h>
#include <minui.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "common.h"

#define SOCK_PATH "/tmp/mint.sock"

struct minui {
    gr_surface surface;
    uint32_t width;
    uint32_t height;
};

static struct minui minui;

static void daemonize(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) exit(EXIT_SUCCESS);

    setsid();
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd >= 0) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > 2) close(fd);
    }
}

static int init_socket(void) {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) return -1;

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);
    unlink(SOCK_PATH);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0 ||
        listen(sockfd, 1) < 0) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

static void render_to_screen(cairo_surface_t *surface) {
    cairo_surface_flush(surface);
    unsigned char *cairo_data = cairo_image_surface_get_data(surface);
    memcpy(minui.surface->data, cairo_data, minui.width * minui.height * 4);
    gr_blit(minui.surface, 0, 0, minui.width, minui.height, 0, 0);
    gr_flip();
}

int main(int argc, char *argv[]) {
    if (argc < 2 ||
        (strcmp(argv[1], "--charging") == 0 && argc < 3) ||
        (strcmp(argv[1], "--text") == 0 && argc < 3) ||
        (strcmp(argv[1], "--splash") != 0 &&
         strcmp(argv[1], "--charging") != 0 &&
         strcmp(argv[1], "--text") != 0)) {

        fprintf(stderr, "Usage: %s --charging BAT_NAME | --splash | --text \"string\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "--text") == 0 && argc >= 3) {
        int sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            return EXIT_FAILURE;
        }

        struct sockaddr_un addr = {0};
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("connect to daemon failed");
            close(sock);
            return EXIT_FAILURE;
        }

        if (write(sock, argv[2], strlen(argv[2])) < 0) {
            perror("write failed");
            close(sock);
            return EXIT_FAILURE;
        }

        close(sock);
        return EXIT_SUCCESS;
    }

    gr_init(false);
    gr_color(0, 0, 0, 255);

    minui.width = gr_fb_width();
    minui.height = gr_fb_height();

    minui.surface = res_malloc_surface(minui.width * minui.height * 4);
    minui.surface->row_bytes = minui.width * 4;
    minui.surface->pixel_bytes = 4;
    minui.surface->width = minui.width;
    minui.surface->height = minui.height;

    double dpi_x = (double)minui.width * 25.4 / gr_fb_mm_width();
    double dpi_y = (double)minui.height * 25.4 / gr_fb_mm_height();
    double physical_scale = ((dpi_x + dpi_y) / 2.0) / 60.0;

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, minui.width, minui.height);
    cairo_t *cr = cairo_create(surface);

    if (strcmp(argv[1], "--charging") == 0) {
        draw_bat(minui.width, minui.height, cr, argv[2], physical_scale);
        render_to_screen(surface);
    } else if (strcmp(argv[1], "--splash") == 0) {
        draw_splash(minui.width, minui.height, cr, physical_scale);
        render_to_screen(surface);

        daemonize();
        int sockfd = init_socket();
        if (sockfd < 0) {
            perror("socket init failed");
            goto cleanup;
        }

        while (1) {
            int client = accept(sockfd, NULL, NULL);
            if (client < 0) {
                perror("accept");
                continue;
            }

            char buf[256] = {0};
            ssize_t n = read(client, buf, sizeof(buf) - 1);
            close(client);

            if (n > 0) {
                double clear_height = 40 * physical_scale;
                cairo_save(cr);
                cairo_rectangle(cr, 0, minui.height - clear_height - 5 * physical_scale,
                                minui.width, clear_height + 10 * physical_scale);
                cairo_fill(cr);
                cairo_restore(cr);

                draw_text_line(cr, minui.width, minui.height, buf, physical_scale);
                render_to_screen(surface);
            }
        }
    }

    pause();

cleanup:
    if (cr) cairo_destroy(cr);
    if (surface) cairo_surface_destroy(surface);
    if (minui.surface) res_free_surface(minui.surface);
    unlink(SOCK_PATH);
    gr_fb_blank(true);
    gr_exit();
    return EXIT_FAILURE;
}
