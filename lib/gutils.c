#include "../include/gutils.h"

GLuint VAOs[GEO_OBJ_MAX];
int vaos_usage[GEO_OBJ_MAX];
GLuint VBOs[GEO_OBJ_MAX];
int vbos_usage[GEO_OBJ_MAX];
GLuint EBOs[GEO_OBJ_MAX];
int ebos_usage[GEO_OBJ_MAX];
GLuint TEXs[GEO_OBJ_MAX];
int texs_usage[GEO_OBJ_MAX];

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

void geo_init() {
	glGenVertexArrays(GEO_OBJ_MAX, VAOs);
	glGenBuffers(GEO_OBJ_MAX, VBOs);
	glGenBuffers(GEO_OBJ_MAX, EBOs);
	glGenTextures(GEO_OBJ_MAX, TEXs);
}

void geo_terminate() {
	glDeleteVertexArrays(GEO_OBJ_MAX, VAOs);
	glDeleteBuffers(GEO_OBJ_MAX, VBOs);
	glDeleteBuffers(GEO_OBJ_MAX, EBOs);
	glDeleteTextures(GEO_OBJ_MAX, TEXs);
}

Geometry geo_create(int arr_buf_cfg[], size_t abc_size, GLfloat vertices[], size_t vertices_size, GLuint indices[], size_t indices_size) {
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

GLuint create_program(const char *vspath, const char *fspath)
{
	GLuint P = glCreateProgram();

	GLuint VS = _create_shader(GL_VERTEX_SHADER, vspath);
	GLuint FS = _create_shader(GL_FRAGMENT_SHADER, fspath);

	glAttachShader(P, VS);
	glAttachShader(P, FS);
	glLinkProgram(P);

	int success;
	glGetProgramiv(P, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char buf[512];
		glGetProgramInfoLog(P, sizeof(buf), NULL, buf);
		gl_shader_err_cback(buf);
		glDeleteProgram(P);
		return 0;
	}
	glDeleteShader(VS);
	glDeleteShader(FS);

	return P;
}

static GLuint _create_shader(GLenum shader_type, const char *shader_src_path)
{
	char buf[512];
	FILE *fsrc = fopen(shader_src_path, "rb");
	int end = fread(buf, 1, sizeof(buf), fsrc);
	buf[end * sizeof(char)] = '\0';
	// printf("%s\n", buf);

	GLuint S = glCreateShader(shader_type);
	const char *pbuf = buf;
	glShaderSource(S, 1, &pbuf, NULL);
	glCompileShader(S);

	int success;
	glGetShaderiv(S, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(S, sizeof(buf), NULL, buf);
		gl_shader_err_cback(buf);
	}

	return S;
}

// Print the error, leaving the choice to exit or not to the main program.
void glfw_err_cback(int ecode, const char *info)
{
	fprintf(stderr, "GLFW Error: %s\n", info);
}

void gl_shader_err_cback(const char *info)
{
	fprintf(stderr, "GL Shader Error: %s\n", info);
}

void glfw_frame_buffer_size_cback(GLFWwindow *wnd, int width, int height)
{
	glViewport(0, 0, width, height);
}

void glfw_key_cback(GLFWwindow *wnd, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		camera_cursor_is_first = 1;
		glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void glfw_mouse_cback(GLFWwindow *wnd, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
		glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void process_input(GLFWwindow* wnd) {
	// camera movement control
	if (glfwGetKey(wnd, GLFW_KEY_W))
		camera_move_forward();
	if (glfwGetKey(wnd, GLFW_KEY_A))
		camera_move_leftward();
	if (glfwGetKey(wnd, GLFW_KEY_S))
		camera_move_backward();
	if (glfwGetKey(wnd, GLFW_KEY_D))
		camera_move_rightward();
	if (glfwGetKey(wnd, GLFW_KEY_SPACE))
		camera_move_upward();
	if (glfwGetKey(wnd, GLFW_KEY_LEFT_SHIFT))
		camera_move_downward();
}