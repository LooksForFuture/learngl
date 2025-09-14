#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#define goto_defer(value) do { exitCode = (value); goto defer; } while(0)

void onWindowResize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	int exitCode = 0;
	GLFWwindow *window = NULL;
	int shouldQuit = 0;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to open window\n");
		goto_defer(-1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		goto_defer(-1);
	}
	glfwSetFramebufferSizeCallback(window, onWindowResize);

	glViewport(0, 0, 800, 600);
	while (!shouldQuit) {
		if (glfwWindowShouldClose(window)) shouldQuit = 1;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			shouldQuit=1;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

defer:
	glfwTerminate();
	return exitCode;
}
