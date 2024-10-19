#include "../include/gutils.h"

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
