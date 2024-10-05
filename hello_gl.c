#include "include/gutils.h"
#include "include/geodata.h" // Contains vertices data, do not multi-ref!

void run(GLFWwindow *wnd)
{
	geo_init();
    GLuint P = create_program("shaders/s1.vert", "shaders/s1.frag");

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(wnd))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }
    geo_terminate();
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