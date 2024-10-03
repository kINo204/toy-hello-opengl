#include "gutils.h"

int cur = 0;

GLfloat coordinates3[] = {
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.0f, 0.0f
};

GLuint indices[] = {
	0, 2, 3,
	1, 2, 4
};

int main() {
	glfwSetErrorCallback(glfw_err_cback);
	if (!glfwInit()) { exit(-1); }

	// Window init
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* wnd = glfwCreateWindow(1024, 768, "Hello GL!", NULL, NULL);
	if (!wnd) {
		glfwTerminate();
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(wnd, glfw_frame_buffer_size_cback);
	glfwSetKeyCallback(wnd, glfw_key_cback);

	glfwMakeContextCurrent(wnd);
	if (!gladLoadGL(glfwGetProcAddress)) {
		fprintf(stderr, "GLAD failed to load in current context!\n");
		exit(-1);
	}

	// Build the shader program.
	GLuint P = glCreateProgram();
    GLuint VS = create_shader(GL_VERTEX_SHADER, "v.vs");
    GLuint FS = create_shader(GL_FRAGMENT_SHADER, "f.fs");
	glAttachShader(P, VS);
	glAttachShader(P, FS);
    glLinkProgram(P);

	int success;
	glGetProgramiv(P, GL_COMPILE_STATUS, &success);
	if (!success) {
		char buf[512];
		glGetProgramInfoLog(P, sizeof(buf), NULL, buf);
		gl_shader_err_cback(buf);
	}
	glDeleteShader(VS);
	glDeleteShader(FS);

	// Create VertexArrays.
	GLuint VAO[2];
	glGenVertexArrays(sizeof(VAO) / sizeof(VAO[0]), VAO);
	GLuint VBO[1];
	glGenBuffers(sizeof(VBO) / sizeof(VBO[0]), VBO);
	GLuint EBO[1];
	glGenBuffers(sizeof(EBO) / sizeof(EBO[0]), EBO);

	// Object a:
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates3), coordinates3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Object b:
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Main window loop
	glfwSwapInterval(1);
	glUseProgram(P);
	while (!glfwWindowShouldClose(wnd)) {
		// Re-drawing here:
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO[cur]);
		if (cur == 0) {
			glDrawArrays(GL_TRIANGLES, 0, 6);
		} else {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	// Exit program
	glDeleteVertexArrays(sizeof(VAO), VAO);
	glDeleteBuffers(sizeof(VBO), VBO);
	glDeleteBuffers(sizeof(EBO), EBO);
	glDeleteProgram(P);
	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}