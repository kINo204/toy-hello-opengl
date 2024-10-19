#include "../include/geometry.h"

GLuint VAOs[GEO_OBJ_MAX];
int vaos_usage[GEO_OBJ_MAX];
GLuint VBOs[GEO_OBJ_MAX];
int vbos_usage[GEO_OBJ_MAX];
GLuint EBOs[GEO_OBJ_MAX];
int ebos_usage[GEO_OBJ_MAX];

void geo_init() {
	glGenVertexArrays(GEO_OBJ_MAX, VAOs);
	glGenBuffers(GEO_OBJ_MAX, VBOs);
	glGenBuffers(GEO_OBJ_MAX, EBOs);
	tex_init();
}

void geo_terminate() {
	glDeleteVertexArrays(GEO_OBJ_MAX, VAOs);
	glDeleteBuffers(GEO_OBJ_MAX, VBOs);
	glDeleteBuffers(GEO_OBJ_MAX, EBOs);
	tex_terminate();
}

Geometry geo_create(int arr_buf_cfg[], size_t abc_size,
					GLfloat vertices[], size_t vertices_size,
					GLuint indices[], size_t indices_size)
{
	Geometry G = (Geometry) malloc(sizeof(struct _Geometry));
	G->VAO = geo_alloc_vao();
	G->VBO = geo_alloc_vbo();
	G->arr_buf_cfg = arr_buf_cfg;
	G->arr_buf_cfg_size = abc_size;
	G->vertices = vertices;
	G->vertices_size = vertices_size;
	G->indices = indices;
	if (indices) {
		G->EBO = geo_alloc_ebo();
		G->indices_size = indices_size;
	} else {
		G->EBO = 0;
		G->indices_size = 0;
	}

	glBindVertexArray(G->VAO);

	// array buffer
	glBindBuffer(GL_ARRAY_BUFFER, G->VBO);
	glBufferData(GL_ARRAY_BUFFER, G->vertices_size, G->vertices, GL_STATIC_DRAW);

	// attribute pointers
	int arr_buf_cfg_len = G->arr_buf_cfg_size / sizeof(int);
	int stride = 0;
	for (int i = 0; i < arr_buf_cfg_len; i++) {
		stride += G->arr_buf_cfg[i];
	}
	int pointer = 0;
	for (int i = 0; i < arr_buf_cfg_len; i++) {
		glVertexAttribPointer(i, G->arr_buf_cfg[i], GL_FLOAT, GL_FALSE,
			sizeof(GLfloat) * stride, (GLfloat*)(sizeof(GLfloat) * pointer));
		pointer += G->arr_buf_cfg[i];
		glEnableVertexAttribArray(i);
	}

	// element array buffer
	if (G->indices) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, G->indices_size, G->indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
	return G;
}

void geo_destroy(Geometry G) {
	if (!G) { return; }
	free(G);
}