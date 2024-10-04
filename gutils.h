#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <linmath.h>

#include <stdio.h>
#include <stdlib.h>

GLuint create_shader(GLenum shader_type, const char *shader_src_path);

void glfw_err_cback(int ecode, const char *info);

void gl_shader_err_cback(const char *info);

void glfw_frame_buffer_size_cback(GLFWwindow *wnd, int width, int height);

void glfw_key_cback(GLFWwindow* wnd, int key, int scancode, int action, int mods);

GLuint create_program(const char *vspath, const char *fspath);