#include "include/gutils.h"
#include "include/geodata.h" // Contains vertices data, do not multi-ref!

// Light source positon.
GLfloat light_pos_x = 0.9f, light_pos_y = 0.4f, light_pos_z = 0.3f;
GLfloat light2_pos_x = 0.9f, light2_pos_y = 0.5f, light2_pos_z = 0.2f;


void run(GLFWwindow *wnd)
{
	geo_init();
	tex_init();

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

	GLuint pic = tex_create("res/textures/id.jpg", GL_RGB);

	GLuint prog_obj = create_program("shaders/pvm.vert", "shaders/object.frag");
	GLuint prog_obj_simple = create_program("shaders/pvm.vert", "shaders/object_simple.frag");
	GLuint prog_obj_envmap = create_program("shaders/pvm.vert", "shaders/object_envmap.frag");
	GLuint prog_src = create_program("shaders/pvm.vert", "shaders/source.frag");
	GLuint prog_light_space = create_program("shaders/light_space.vert", "shaders/empty.frag");

	// Shadow utils.
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depth_map_fbo[2], depth_map[2];
	glGenFramebuffers(2, &depth_map_fbo[0]);
	glGenTextures(2, &depth_map[0]);

	glBindTexture(GL_TEXTURE_2D, depth_map[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, depth_map[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Environment mapping utils.
	GLuint cube_tex, cube_fbo;
	glGenFramebuffers(1, &cube_fbo);
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex);
	for (GLuint i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Init trans mat: projection, view
	mat4x4 p, v;
	mat4x4_perspective(p, 1.047f, 1.f, 0.1f, 100.f); // Fixed at the time.
	camera_init();
	camera_update_view_trans(v);

	// Update light view trans-mat.
	mat4x4 vl, vl2;
	mat4x4_look_at(vl, (vec3){light_pos_x, light_pos_y, light_pos_z}, (vec3){0.f, 0.f, 0.f}, camera.up);
	mat4x4_look_at(vl2, (vec3){light2_pos_x, light2_pos_y, light2_pos_z}, (vec3){0.f, 0.f, 0.f}, camera.up);

	// Init trans mat: model(s)
	mat4x4 m1, m2;
	float scale_rate = 0.25f;
	mat4x4_identity(m1);
	mat4x4_translate_in_place(m1, -0.25f, -0.16f, 0.f);
	mat4x4_scale_aniso(m1, m1, scale_rate, scale_rate, scale_rate);
	scale_rate = 0.2f;
	mat4x4_identity(m2);
	mat4x4_translate_in_place(m2, 0.25f, 0.f, 0.f);
	mat4x4_scale_aniso(m2, m2, scale_rate, scale_rate, scale_rate);

	// Begin drawing procedure.
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST); // Enable depth test when drawing.
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	GLfloat delta_time = 0.f;
	GLfloat lastframe_time = 0.f;

	while (!glfwWindowShouldClose(wnd))
    {
		GLfloat curframe_time = glfwGetTime();
		delta_time = curframe_time - lastframe_time;
		lastframe_time = curframe_time;
		CAMERA_SPEED = 3.5f * delta_time;

		process_input(wnd);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind the cube framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, cube_fbo);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cube_tex, 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint prog = prog_obj_simple;
		glUseProgram(prog);
			glUniform3f(glGetUniformLocation(prog, "light[0].ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(prog, "light[0].diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(prog, "light[0].specular"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(prog, "light[0].pos"), light_pos_x, light_pos_y, light_pos_z);
			glUniform3f(glGetUniformLocation(prog, "light[1].ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(prog, "light[1].diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(prog, "light[1].specular"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(prog, "light[1].pos"), light2_pos_x, light2_pos_y, light2_pos_z);
			glUniformMatrix4fv(glGetUniformLocation(prog, "proj"), 1, GL_FALSE, (GLfloat *)p);
			mat4x4 v_cube;
			vec3 cube_pos = {0.15f, 0.f, 0.f}, cube_target = {0.f, 0.f, 0.f};
			vec3_reflect(cube_target, camera.dir, (vec3){-1.f, 0.f, 0.f});
			vec3_add(cube_target, cube_pos, cube_target);
			mat4x4_look_at(v_cube, cube_pos, cube_target, camera.up);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view"), 1, GL_FALSE, (GLfloat *)v_cube);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view_light_space[0]"), 1, GL_FALSE, (GLfloat *)vl);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view_light_space[1]"), 1, GL_FALSE, (GLfloat *)vl2);
			glUniform3fv(glGetUniformLocation(prog, "camera_pos"), 1, cube_pos);

		glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, (GLfloat *)m1);
			glUniform3f(glGetUniformLocation(prog, "material.ambient"), 1.f, 0.5f, 0.31f);
			glUniform3f(glGetUniformLocation(prog, "material.diffuse"), 1.f, 0.5f, 0.31f);
			glUniform3f(glGetUniformLocation(prog, "material.specular"), 0.5f, 0.5f, 0.5f);
			glUniform1f(glGetUniformLocation(prog, "material.shininess"), 32.f);
			glUniform1f(glGetUniformLocation(prog, "material.alpha"), 1.f);
		glBindVertexArray(obj->VAO);
		glDrawArrays(GL_TRIANGLES, 0, len_ball);


		// Bind the shadow framebuffer.
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo[0]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map[0], 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glClear(GL_DEPTH_BUFFER_BIT);

		// Draw the shadow framebuffer.
		glUseProgram(prog_light_space);
			glUniformMatrix4fv(glGetUniformLocation(prog_light_space, "proj"), 1, GL_FALSE, (GLfloat*)p);
			glUniformMatrix4fv(glGetUniformLocation(prog_light_space, "view_light_space"), 1, GL_FALSE, (GLfloat*)vl);

		glUniformMatrix4fv(glGetUniformLocation(prog_light_space, "model"), 1, GL_FALSE, (GLfloat*)m1);
		glBindVertexArray(obj->VAO);
		glDrawArrays(GL_TRIANGLES, 0, len_ball);

		glUniformMatrix4fv(glGetUniformLocation(prog_light_space, "model"), 1, GL_FALSE, (GLfloat*)m2);
		glBindVertexArray(obj2->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Bind the shadow framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo[1]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map[1], 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glClear(GL_DEPTH_BUFFER_BIT);

		// Draw the shadow framebuffer.
		glUniformMatrix4fv(glGetUniformLocation(prog_light_space, "view_light_space"), 1, GL_FALSE, (GLfloat*)vl2);

		glUniformMatrix4fv(glGetUniformLocation(prog_light_space, "model"), 1, GL_FALSE, (GLfloat*)m1);
		glBindVertexArray(obj->VAO);
		glDrawArrays(GL_TRIANGLES, 0, len_ball);

		glUniformMatrix4fv(glGetUniformLocation(prog_light_space, "model"), 1, GL_FALSE, (GLfloat*)m2);
		glBindVertexArray(obj2->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw the real framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int wnd_size[2];
		glfwGetWindowSize(wnd, &wnd_size[0], &wnd_size[1]);
		glViewport(0, 0, wnd_size[0], wnd_size[1]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depth_map[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depth_map[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, pic);
		camera_update_view_trans(v);

		// Perform real drawing.
		/* Note: uniform variables MUST be passed afting binding the corresponding program! */
		prog = prog_obj;
		glUseProgram(prog);
			glUniform1i(glGetUniformLocation(prog, "shadow_map"), 0);
			glUniform1i(glGetUniformLocation(prog, "shadow_map2"), 1);
			glUniform3f(glGetUniformLocation(prog, "light[0].ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(prog, "light[0].diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(prog, "light[0].specular"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(prog, "light[0].pos"), light_pos_x, light_pos_y, light_pos_z);
			glUniform3f(glGetUniformLocation(prog, "light[1].ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(prog, "light[1].diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(prog, "light[1].specular"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(prog, "light[1].pos"), light2_pos_x, light2_pos_y, light2_pos_z);
			glUniformMatrix4fv(glGetUniformLocation(prog, "proj"), 1, GL_FALSE, (GLfloat *)p);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view"), 1, GL_FALSE, (GLfloat *)v);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view_light_space[0]"), 1, GL_FALSE, (GLfloat *)vl);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view_light_space[1]"), 1, GL_FALSE, (GLfloat *)vl2);
			glUniform3fv(glGetUniformLocation(prog, "camera_pos"), 1, camera.pos);

		glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, (GLfloat *)m1);
			glUniform3f(glGetUniformLocation(prog, "material.ambient"), 1.f, 0.5f, 0.31f);
			glUniform3f(glGetUniformLocation(prog, "material.diffuse"), 1.f, 0.5f, 0.31f);
			glUniform3f(glGetUniformLocation(prog, "material.specular"), 0.5f, 0.5f, 0.5f);
			glUniform1f(glGetUniformLocation(prog, "material.shininess"), 32.f);
			glUniform1f(glGetUniformLocation(prog, "material.alpha"), 1.f);
		glBindVertexArray(obj->VAO);
		glDrawArrays(GL_TRIANGLES, 0, len_ball);

		prog = prog_obj_envmap;
		glUseProgram(prog);
			glUniform1i(glGetUniformLocation(prog, "shadow_map"), 0);
			glUniform1i(glGetUniformLocation(prog, "shadow_map2"), 1);
			glUniform1i(glGetUniformLocation(prog, "envmap"), 2);
			glUniform1i(glGetUniformLocation(prog, "tex"), 3);
			glUniform3f(glGetUniformLocation(prog, "light[0].ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(prog, "light[0].diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(prog, "light[0].specular"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(prog, "light[0].pos"), light_pos_x, light_pos_y, light_pos_z);
			glUniform3f(glGetUniformLocation(prog, "light[1].ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(prog, "light[1].diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(prog, "light[1].specular"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(prog, "light[1].pos"), light2_pos_x, light2_pos_y, light2_pos_z);
			glUniformMatrix4fv(glGetUniformLocation(prog, "proj"), 1, GL_FALSE, (GLfloat *)p);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view"), 1, GL_FALSE, (GLfloat *)v);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view_light_space[0]"), 1, GL_FALSE, (GLfloat *)vl);
			glUniformMatrix4fv(glGetUniformLocation(prog, "view_light_space[1]"), 1, GL_FALSE, (GLfloat *)vl2);
			glUniform3fv(glGetUniformLocation(prog, "camera_pos"), 1, camera.pos);

		glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, (GLfloat*)m2);
			glUniform3f(glGetUniformLocation(prog, "material.ambient"), 0.24f, 1.f, 0.78f);
			glUniform3f(glGetUniformLocation(prog, "material.diffuse"), 0.24f, 1.f, 0.78f);
			glUniform3f(glGetUniformLocation(prog, "material.specular"), 0.5f, 0.5f, 0.5f);
			glUniform1f(glGetUniformLocation(prog, "material.shininess"), 32.f);
			glUniform1f(glGetUniformLocation(prog, "material.alpha"), 1.f);
		glBindVertexArray(obj2->VAO);
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