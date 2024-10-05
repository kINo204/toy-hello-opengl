#include "include/gutils.h"
#include "include/geodata.h" // Contains vertices data, do not multi-ref!

void run(GLFWwindow *wnd)
{
	geo_init();

	Geometry cube = geo_create(
		cube_cfg, sizeof(cube_cfg),
		cube_vert, sizeof(cube_vert),
		NULL, 0);
	GLuint simple_program = create_program("shaders/pvm.vert", "shaders/textured.frag");
	glActiveTexture(GL_TEXTURE0);
	GLuint texture_box = tex_create("res/textures/container.jpg", GL_RGB);

	camera_init();

	mat4x4 p, v, m1, m2;
	mat4x4_perspective(p, 1.047f, 1.f, 0.1f, 100.f);
	camera_update_view_trans(v);

	float scale_rate = 1.0f;
	mat4x4_identity(m1);
	mat4x4_translate_in_place(m1, -0.5f, 0.f, -0.5f);
	mat4x4_scale_aniso(m1, m1, scale_rate, scale_rate, scale_rate);
	mat4x4_identity(m2);
	mat4x4_rotate_Y(m2, m2, -0.52f);
	mat4x4_translate_in_place(m2, 0.6f, 0.f, 0.6f);
	mat4x4_scale_aniso(m2, m2, scale_rate, scale_rate, scale_rate);

	
	GLuint up, uv, um;
	up = glGetUniformLocation(simple_program, "proj");
	uv = glGetUniformLocation(simple_program, "view");
	um = glGetUniformLocation(simple_program, "model");

	glUseProgram(simple_program);
	glBindVertexArray(cube->VAO);

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
	GLfloat delta_time = 0.f;
	GLfloat lastframe_time = 0.f;
	while (!glfwWindowShouldClose(wnd))
    {
		GLfloat curframe_time = glfwGetTime();
		delta_time = curframe_time - lastframe_time;
		lastframe_time = curframe_time;
		CAMERA_SPEED = 3.5f * delta_time;

		process_input(wnd);

		camera_update_view_trans(v);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update PVM transforms.
		glUniformMatrix4fv(up, 1, GL_FALSE, (GLfloat*)p);
		glUniformMatrix4fv(uv, 1, GL_FALSE, (GLfloat*)v);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_box);

		glUniformMatrix4fv(um, 1, GL_FALSE, (GLfloat*)m1);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniformMatrix4fv(um, 1, GL_FALSE, (GLfloat*)m2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }
    geo_terminate();
}

int main()
{
	// STBI init
	// stbi_set_flip_vertically_on_load(1);

	// GLFW init
	glfwSetErrorCallback(glfw_err_cback);
	if (!glfwInit()) { exit(-1); }

	// Window init
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *wnd = glfwCreateWindow(1400, 1400, "Hello GL!", NULL, NULL);
	if (!wnd)
	{
		glfwTerminate();
		exit(-1);
	}
	GLFWimage wnd_icon;
	int trash;
	wnd_icon.pixels = stbi_load("res/textures/awesomeface.png", &(wnd_icon.width), &(wnd_icon.height), &trash, 0);
	if (!wnd_icon.pixels) {
		fprintf(stderr, "STBI Error: failed to load wnd icon!\n");
	} else {
		glfwSetWindowIcon(wnd, 1, &wnd_icon);
	}
	glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(wnd, camera_cursor_cback);
	glfwSetMouseButtonCallback(wnd, glfw_mouse_cback);
	glfwSetFramebufferSizeCallback(wnd, glfw_frame_buffer_size_cback);
	glfwSetKeyCallback(wnd, glfw_key_cback);
	glfwMakeContextCurrent(wnd);

	// GLAD load OpenGL
	if (!gladLoadGL(glfwGetProcAddress))
	{
		fprintf(stderr, "GLAD failed to load in current context!\n");
		exit(-1);
	}

    run(wnd);

    glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}