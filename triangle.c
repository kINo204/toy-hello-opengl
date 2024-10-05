#include "include/gutils.h"
#include "include/geodata.h" // Contains vertices data, do not multi-ref!

void run(GLFWwindow *wnd)
{
	geo_init();

    // objects
    GLuint TEX[2]; glGenTextures(sizeof(TEX) / sizeof(TEX[0]), TEX);

    // Shader programs
    GLuint P = create_program("shaders/s1.vert", "shaders/s1.frag");

    // Geometries
	Geometry G1 = geo_create(
		g1_cfg, sizeof(g1_cfg),
		g1_vertices, sizeof(g1_vertices),
		g1_indices, sizeof(g1_indices));

    // Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TEX[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    {
        int x, y, channels_in_file;
        stbi_uc *img = stbi_load("res/textures/container.jpg", &x, &y, &channels_in_file, 0);
        if (!img)
        {
            fprintf(stderr, "STBI Error: failed to load img!\n");
        }
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
        if (!img)
        {
            fprintf(stderr, "STBI Error: failed to load img!\n");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(img);
    };

    // Edit prog uniforms.
    glUseProgram(P);
    glUniform1i(glGetUniformLocation(P, "our_tex"), 0);
    glUniform1i(glGetUniformLocation(P, "our_tex1"), 1);

    // Coordinate transformations
    mat4x4 ident, model, view, proj;
    mat4x4_identity(ident);

    mat4x4_scale_aniso(model, ident, 0.5, 0.5, 0.5);

    mat4x4_identity(view);
    mat4x4_translate_in_place(view, 0, 0, -0.8);
    mat4x4_rotate_X(view, view, -0.7);

    mat4x4_perspective(proj, 1.04f, 1.0f, 0.1f, 100.0f);

    GLuint um, uv, up;
    um = glGetUniformLocation(P, "model");
    uv = glGetUniformLocation(P, "view");
    up = glGetUniformLocation(P, "proj");

    mat4x4 TransMatrix;
    mat4x4_identity(TransMatrix);
    GLuint umytrans = glGetUniformLocation(P, "trans_mat");

    // Main window loop
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(wnd))
    {
        // Re-drawing here:
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(P); // Constant shading program.
        mat4x4_rotate_Z(TransMatrix, TransMatrix, 0.01f);
        glUniformMatrix4fv(umytrans, 1, GL_FALSE, (GLfloat *)TransMatrix);
        glUniformMatrix4fv(um, 1, GL_FALSE, (GLfloat*)model);
        glUniformMatrix4fv(uv, 1, GL_FALSE, (GLfloat*)view);
        glUniformMatrix4fv(up, 1, GL_FALSE, (GLfloat*)proj);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TEX[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, TEX[1]);

		geo_bind(G1);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }
}

int main()
{
	// STBI init
	stbi_set_flip_vertically_on_load(1);

	// GLFW init
	glfwSetErrorCallback(glfw_err_cback);
	if (!glfwInit()) { exit(-1); }

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