#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <linmath.h>

#include <stdio.h>
#include <stdlib.h>

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

Geometry geo_create(int arr_buf_cfg[], size_t abc_size, GLfloat vertices[], size_t vertices_size, GLuint indices[], size_t indices_size);

void geo_bind(Geometry G);

void geo_destroy(Geometry G);

void glfw_err_cback(int ecode, const char *info);

void gl_shader_err_cback(const char *info);

void glfw_frame_buffer_size_cback(GLFWwindow *wnd, int width, int height);

void glfw_key_cback(GLFWwindow* wnd, int key, int scancode, int action, int mods);

GLuint create_program(const char *vspath, const char *fspath);

// Static
static GLuint _create_shader(GLenum shader_type, const char *shader_src_path);

#define GEO_OBJ_MAX 10
extern GLuint VAOs[GEO_OBJ_MAX];
extern int vaos_usage[GEO_OBJ_MAX];
extern GLuint VBOs[GEO_OBJ_MAX];
extern int vbos_usage[GEO_OBJ_MAX];
extern GLuint EBOs[GEO_OBJ_MAX];
extern int ebos_usage[GEO_OBJ_MAX];

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