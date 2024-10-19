#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <linmath.h>

#include <stdio.h>
#include <stdlib.h>

// Components used:
#include "geometry.h"
#include "camera.h"
#include "tex.h"

void process_input(GLFWwindow* wnd);
GLuint create_program(const char *vspath, const char *fspath);

void glfw_err_cback(int ecode, const char *info);
void gl_shader_err_cback(const char *info);
void glfw_frame_buffer_size_cback(GLFWwindow *wnd, int width, int height);
void glfw_key_cback(GLFWwindow* wnd, int key, int scancode, int action, int mods);
void glfw_mouse_cback(GLFWwindow *wnd, int button, int action, int mods);