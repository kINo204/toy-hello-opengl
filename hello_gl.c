#include "include/gutils.h"
#include "include/geodata.h" // Contains vertices data, do not multi-ref!

void run(GLFWwindow *wnd)
{
	geo_init();

	// Create entities: programs, textures, geometries
	Geometry obj = geo_create(cfg_ball, sizeof(cfg_ball),
							v_ball, sizeof(v_ball),
							NULL, 0);

	Geometry obj2 = geo_create(cfg_obj, sizeof(cfg_obj),
							v_obj, sizeof(v_obj),
							NULL, 0);

	Geometry src = geo_create(cfg_src, sizeof(cfg_src),
							v_src, sizeof(v_src),
							NULL, 0);

	GLuint prog_obj = create_program("shaders/pvm.vert", "shaders/object.frag");
	GLuint u_obj_proj, u_obj_view, u_obj_model,
		u_obj_obj_ambient, u_obj_obj_diffuse, u_obj_obj_specular, u_obj_obj_shininess,
		u_obj_light_ambient, u_obj_light_diffuse, u_obj_light_specular, u_obj_light_pos,
		u_obj_camera_pos;
	u_obj_proj = glGetUniformLocation(prog_obj, "proj");
	u_obj_view = glGetUniformLocation(prog_obj, "view");
	u_obj_model = glGetUniformLocation(prog_obj, "model");
	u_obj_obj_ambient = glGetUniformLocation(prog_obj, "material.ambient");
	u_obj_obj_diffuse = glGetUniformLocation(prog_obj, "material.diffuse");
	u_obj_obj_specular = glGetUniformLocation(prog_obj, "material.specular");
	u_obj_obj_shininess = glGetUniformLocation(prog_obj, "material.shininess");
	u_obj_light_ambient = glGetUniformLocation(prog_obj, "light.ambient");
	u_obj_light_diffuse = glGetUniformLocation(prog_obj, "light.diffuse");
	u_obj_light_specular = glGetUniformLocation(prog_obj, "light.specular");
	u_obj_light_pos = glGetUniformLocation(prog_obj, "light.pos");
	u_obj_camera_pos = glGetUniformLocation(prog_obj, "camera_pos");

	GLuint prog_src = create_program("shaders/pvm.vert", "shaders/source.frag");
	GLuint u_src_proj, u_src_view, u_src_model;
	u_src_proj = glGetUniformLocation(prog_src, "proj");
	u_src_view = glGetUniformLocation(prog_src, "view");
	u_src_model = glGetUniformLocation(prog_src, "model");

	// Init trans mat: projection, view
	mat4x4 p, v;
	mat4x4_perspective(p, 1.047f, 1.f, 0.1f, 100.f); // Fixed at the time.
	camera_init();
	camera_update_view_trans(v);

	// Init trans mat: model(s)
	mat4x4 m1, m2;
	float scale_rate = 0.5f;
	mat4x4_identity(m1);
	mat4x4_translate_in_place(m1, -0.5f, 0.f, 0.f);
	mat4x4_scale_aniso(m1, m1, scale_rate, scale_rate, scale_rate);
	scale_rate = 0.5f;
	mat4x4_identity(m2);
	mat4x4_translate_in_place(m2, 0.5f, 0.f, 0.f);
	mat4x4_scale_aniso(m2, m2, scale_rate, scale_rate, scale_rate);

	// Begin drawing procedure.
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST); // Enable depth test when drawing.

	GLfloat delta_time = 0.f;
	GLfloat lastframe_time = 0.f;

	// Setting invariant states.
	glUseProgram(prog_obj);
		glUniformMatrix4fv(u_obj_proj, 1, GL_FALSE, (GLfloat*)p);
		glUniform3f(u_obj_obj_ambient, 1.f, 0.5f, 0.31f);
		glUniform3f(u_obj_obj_diffuse, 1.f, 0.5f, 0.31f);
		glUniform3f(u_obj_obj_specular, 0.5f, 0.5f, 0.5f);
		glUniform1f(u_obj_obj_shininess, 32.f);
		glUniform3f(u_obj_light_ambient, 0.2f, 0.2f, 0.2f);
		glUniform3f(u_obj_light_diffuse, 0.5f, 0.5f, 0.5f);
		glUniform3f(u_obj_light_specular, 1.f, 1.f, 1.f);
		glUniform3f(u_obj_light_pos, 0.6f, 0.f, 0.6f);
	// glUseProgram(prog_src);
	// 	glUniformMatrix4fv(u_src_proj, 1, GL_FALSE, (GLfloat*)p);
	// 	glUniformMatrix4fv(u_src_model, 1, GL_FALSE, (GLfloat*)m2);

	while (!glfwWindowShouldClose(wnd))
    {
		GLfloat curframe_time = glfwGetTime();
		delta_time = curframe_time - lastframe_time;
		lastframe_time = curframe_time;
		CAMERA_SPEED = 3.5f * delta_time;

		process_input(wnd);
		camera_update_view_trans(v);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Perform drawing.
		/* Note: uniform variables MUST be passed afting binding the corresponding program! */
		glUseProgram(prog_obj);
			glUniformMatrix4fv(u_obj_view, 1, GL_FALSE, (GLfloat*)v);
			glUniform3fv(u_obj_camera_pos, 1, camera.pos);
			glUniformMatrix4fv(u_obj_model, 1, GL_FALSE, (GLfloat*)m1);

		glBindVertexArray(obj->VAO);
		glDrawArrays(GL_TRIANGLES, 0, len_ball);

			glUniformMatrix4fv(u_obj_model, 1, GL_FALSE, (GLfloat*)m2);
		glBindVertexArray(obj2->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// glUseProgram(prog_src);
		// 	glUniformMatrix4fv(u_src_view, 1, GL_FALSE, (GLfloat*)v);
		// glBindVertexArray(src->VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 36);

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