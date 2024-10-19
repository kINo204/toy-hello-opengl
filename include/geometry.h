#pragma once

#include "gutils.h"

#ifndef GEO_OBJ_MAX
#define GEO_OBJ_MAX 10
#endif

extern GLuint VAOs[GEO_OBJ_MAX];
extern int vaos_usage[GEO_OBJ_MAX];
extern GLuint VBOs[GEO_OBJ_MAX];
extern int vbos_usage[GEO_OBJ_MAX];
extern GLuint EBOs[GEO_OBJ_MAX];
extern int ebos_usage[GEO_OBJ_MAX];

typedef struct _Geometry {
    // objects
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    // data
    int* arr_buf_cfg;
    size_t arr_buf_cfg_size;
    GLfloat* vertices;
    size_t vertices_size;
    GLuint* indices;
    size_t indices_size;
} *Geometry;

void geo_init();
void geo_terminate();
Geometry geo_create(int arr_buf_cfg[], size_t abc_size,
                    GLfloat vertices[], size_t vertices_size,
                    GLuint indices[], size_t indices_size);
void geo_destroy(Geometry G);

// Helpers.
static inline GLuint geo_alloc_vao() {
    static int ind = 0;
    if (ind >= GEO_OBJ_MAX) {
        fprintf(stderr, "GEO Error: no VAO available!\n");
        return 0;
    }
    vaos_usage[ind] = 1;
    return VAOs[ind++];
}

static inline GLuint geo_alloc_vbo() {
    static int ind = 0;
    if (ind >= GEO_OBJ_MAX) {
        fprintf(stderr, "GEO Error: no VAO available!\n");
        return 0;
    }
    vbos_usage[ind] = 1;
    return VBOs[ind++];
}

static inline GLuint geo_alloc_ebo() {
    static int ind = 0;
    if (ind >= GEO_OBJ_MAX) {
        fprintf(stderr, "GEO Error: no VAO available!\n");
        return 0;
    }
    ebos_usage[ind] = 1;
    return EBOs[ind++];
}