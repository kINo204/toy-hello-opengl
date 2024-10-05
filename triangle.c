#include "include/gutils.h"
#include "include/geodata.h" // Contains vertices data, do not multi-ref!

void run(GLFWwindow *wnd)
{
	geo_init();

    // objects
    GLuint TEX[2]; glGenTextures(sizeof(TEX) / sizeof(TEX[0]), TEX);

    // Shader programs
    GLuint P = create_program("shaders/s1.vert", "shaders/s1.frag");
    GLuint P2 = create_program("shaders/s2.vert", "shaders/s1.frag");

    // Geometries
    Geometry G2 = geo_create(
        g2_cfg, sizeof(g2_cfg),
        g2_vertices, sizeof(g2_vertices),
        NULL, 0);

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
    glUseProgram(P2);
    glUniform1i(glGetUniformLocation(P2, "our_tex"), 0);
    glUniform1i(glGetUniformLocation(P2, "our_tex1"), 1);

    // Model trans
    mat4x4 model_box;
    mat4x4_identity(model_box);
    mat4x4_scale_aniso(model_box, model_box, 0.4, 0.4, 0.4);

    mat4x4 model_tri;
    mat4x4_identity(model_tri);
    mat4x4_rotate_X(model_tri, model_tri, 1.57f);
    mat4x4_scale_aniso(model_tri, model_tri, 0.4, 0.4, 0.4);
    mat4x4_translate_in_place(model_tri, 0.f, 0.f, 0.5f);

    // Camara trans
    mat4x4 view, proj;
    mat4x4_identity(view);
    mat4x4_translate_in_place(view, 0, 0, -0.8);
    mat4x4_rotate_X(view, view, -0.58);
    mat4x4_perspective(proj, 1.24f, 1.0f, 0.1f, 100.0f);

    GLuint um, uv, up;
    um = glGetUniformLocation(P, "model");
    uv = glGetUniformLocation(P, "view");
    up = glGetUniformLocation(P, "proj");
    GLuint um2, uv2, up2;
    um2 = glGetUniformLocation(P2, "model");
    uv2 = glGetUniformLocation(P2, "view");
    up2 = glGetUniformLocation(P2, "proj");

    mat4x4 trans1;
    mat4x4_identity(trans1);
    mat4x4 trans2;
    mat4x4_identity(trans2);
    GLuint umytrans = glGetUniformLocation(P, "trans_mat");
    GLuint umytrans2 = glGetUniformLocation(P2, "trans_mat");

    // Main window loop
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(wnd))
    {
        // Re-drawing here:
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mat4x4_rotate_Z(trans1, trans1,  0.01f);
        mat4x4_rotate_Z(trans2, trans2, -0.01f);

        geo_bind(G2);

            glUseProgram(P); // Constant shading program.
            glUniformMatrix4fv(umytrans, 1, GL_FALSE, (GLfloat *)trans1);
            glUniformMatrix4fv(um, 1, GL_FALSE, (GLfloat*)model_box);
            glUniformMatrix4fv(uv, 1, GL_FALSE, (GLfloat*)view);
            glUniformMatrix4fv(up, 1, GL_FALSE, (GLfloat*)proj);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TEX[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, TEX[1]);
        glDrawArrays(GL_TRIANGLES, 12, 36);

            glUseProgram(P2); // Constant shading program.
            glUniformMatrix4fv(umytrans2, 1, GL_FALSE, (GLfloat *)trans2);
            glUniformMatrix4fv(um2, 1, GL_FALSE, (GLfloat*)model_tri);
            glUniformMatrix4fv(uv2, 1, GL_FALSE, (GLfloat*)view);
            glUniformMatrix4fv(up2, 1, GL_FALSE, (GLfloat*)proj);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TEX[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, TEX[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }

    geo_destroy(G2);
    glDeleteProgram(P);
    glDeleteProgram(P2);
    geo_terminate();
    glDeleteTextures(sizeof(TEX) / sizeof(TEX[0]), TEX);
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