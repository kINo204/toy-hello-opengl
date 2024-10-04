#include "gutils.h"

GLfloat vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3
};

int main()
{
	stbi_set_flip_vertically_on_load(1);
	glfwSetErrorCallback(glfw_err_cback);
	if (!glfwInit())
	{
		exit(-1);
	}

	// Window init
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *wnd = glfwCreateWindow(1024, 1024, "Hello GL!", NULL, NULL);
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

	// Create objects
	GLuint VAO[1];
	glGenVertexArrays(sizeof(VAO) / sizeof(VAO[0]), VAO);
	GLuint VBO[1];
	glGenBuffers(sizeof(VBO) / sizeof(VBO[0]), VBO);
	GLuint EBO[1];
	glGenBuffers(sizeof(EBO) / sizeof(EBO[0]), EBO);
	GLuint TEX[2];
	glGenTextures(sizeof(TEX) / sizeof(TEX[0]), TEX);

	// Geometry
	glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // pos
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // color
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // tex-coord
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Texture
	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEX[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			{
				int x, y, channels_in_file;
				stbi_uc *img = stbi_load("res/textures/container.jpg", &x, &y, &channels_in_file, 0);
				if (!img) { fprintf(stderr, "STBI Error: failed to load img!\n"); }
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(img);
			};

	glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TEX[1]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			{
				int x, y, channels_in_file;
				stbi_uc *img = stbi_load("res/textures/awesomeface.png", &x, &y, &channels_in_file, 0);
				if (!img) { fprintf(stderr, "STBI Error: failed to load img!\n"); }
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(img);
			};
	
	// Edit prog uniforms.
	glUseProgram(P);
	glUniform1i(glGetUniformLocation(P, "our_tex"), 0);
	glUniform1i(glGetUniformLocation(P, "our_tex1"), 1);

	mat4x4 TransMatrix;
	mat4x4_identity(TransMatrix);
	glUseProgram(P);
	GLuint uni_trans_mat = glGetUniformLocation(P, "trans_mat");
	glUniformMatrix4fv(uni_trans_mat, 1, GL_FALSE, (GLfloat*)TransMatrix);

	// Main window loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(wnd))
	{
		// Re-drawing here:
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(P); // Constant shading program.

		mat4x4_rotate_Z(TransMatrix, TransMatrix, 0.01f);
		glUniformMatrix4fv(uni_trans_mat, 1, GL_FALSE, (GLfloat*)TransMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEX[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TEX[1]);

		glBindVertexArray(VAO[0]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	// Exit program
	glDeleteVertexArrays(sizeof(VAO), VAO);
	glDeleteBuffers(sizeof(VBO), VBO);
	glDeleteBuffers(sizeof(EBO), EBO);
	glDeleteTextures(sizeof(TEX), TEX);
	glDeleteProgram(P);
	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}