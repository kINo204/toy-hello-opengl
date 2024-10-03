#include "gutils.h"

GLfloat vertices[] = {
	// positions         // colors
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f	  // top
};

GLuint indices[] = {
	0, 1, 2};

int main()
{
	glfwSetErrorCallback(glfw_err_cback);
	if (!glfwInit())
	{
		exit(-1);
	}

	// Window init
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *wnd = glfwCreateWindow(1024, 768, "Hello GL!", NULL, NULL);
	if (!wnd)
	{
		glfwTerminate();
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(wnd, glfw_frame_buffer_size_cback);
	glfwSetKeyCallback(wnd, glfw_key_cback);

	glfwMakeContextCurrent(wnd);
	if (!gladLoadGL(glfwGetProcAddress))
	{
		fprintf(stderr, "GLAD failed to load in current context!\n");
		exit(-1);
	}

	// Build the shader program.
	GLuint P = create_program("shaders/vertex_shader.vert", "shaders/frag_shader.frag");

	// Create VertexArrays.
	GLuint VAO[1];
	glGenVertexArrays(sizeof(VAO) / sizeof(VAO[0]), VAO);
	GLuint VBO[2];
	glGenBuffers(sizeof(VBO) / sizeof(VBO[0]), VBO);
	GLuint EBO[1];
	glGenBuffers(sizeof(EBO) / sizeof(EBO[0]), EBO);

	// The colored object
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Main window loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(wnd))
	{
		// Re-drawing here:
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(P); // Constant shading program.
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

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