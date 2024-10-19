#include "../include/tex.h"

GLuint TEXs[GEO_OBJ_MAX];
int texs_usage[GEO_OBJ_MAX];

void tex_init() {
	glGenTextures(GEO_OBJ_MAX, TEXs);
}

void tex_terminate() {
	glDeleteTextures(GEO_OBJ_MAX, TEXs);
}

GLuint tex_create(const char *img, GLenum channel_type) {
	GLuint T = tex_alloc();
	glBindTexture(GL_TEXTURE_2D, T);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(img, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channel_type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// printf("STBI load img %s successful\n", img);
	}
	else {
		fprintf(stderr, "STBI Error: failed to load img!\n");
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return T;
}