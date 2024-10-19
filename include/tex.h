#pragma once

#include "gutils.h"

#ifndef GEO_OBJ_MAX
#define GEO_OBJ_MAX 10
#endif

extern GLuint TEXs[GEO_OBJ_MAX];
extern int texs_usage[GEO_OBJ_MAX];

void tex_init();
void tex_terminate();
GLuint tex_create(const char *img, GLenum channel_type);

static inline GLuint tex_alloc() {
    static int ind = 0;
    if (ind >= GEO_OBJ_MAX) {
        fprintf(stderr, "GEO Error: no VAO available!\n");
        return 0;
    }
    texs_usage[ind] = 1;
    return TEXs[ind++];
}