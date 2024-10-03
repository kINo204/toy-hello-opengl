#include "gutils.h"

GLuint create_shader(GLenum shader_type, const char* shader_src_path)
{
	char buf[512];
	FILE *fsrc = fopen(shader_src_path, "rb");
	int end = fread(buf, 1, sizeof(buf), fsrc);
	buf[end * sizeof(char)] = '\0';
	// printf("%s\n", buf);

	GLuint S = glCreateShader(shader_type);
	const char* pbuf = buf;
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
void glfw_err_cback(int ecode, const char* info) {
	fprintf(stderr, "GLFW Error: %s\n", info);
}

void gl_shader_err_cback(const char* info) {
	fprintf(stderr, "GL Shader Error: %s\n", info);
}

void glfw_frame_buffer_size_cback(GLFWwindow* wnd, int width, int height) {
	glViewport(0, 0, width, height);
}

extern int cur;
void glfw_key_cback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		cur = (cur == 1) ? 0 : 1;
	}
}