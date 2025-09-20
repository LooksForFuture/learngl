#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define LGL_SHADER_IMPLEMENTATION
#include <lgl_shader.h>

#include <math.h>
#include <stdio.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define goto_defer(value) do { exitCode = (value); goto defer; } while (0);

void onWindowResize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	int exitCode = 0, glLoaded = 0, shouldQuit = 0;
	GLFWwindow *window;
	unsigned int vertexShader, fragmentShader, shaderProgram;
	unsigned int VAO, VBO;

	float vertices[] = {
		/* positions */     /* colors */
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, /* bottom right */
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, /* bottom left */
		0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f /* top */
	};

	float xOffset = 0;
	int xOffsetLocation = 0;
	float currentTime, previousTime, deltaTime = 0, timePassed = 0;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Could not create window\n");
		goto_defer(-1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Could not load OpenGL\n");
		goto_defer(-1);
	}
	glLoaded = 1;
	glfwSetFramebufferSizeCallback(window, onWindowResize);

	/* shader creation
	   --------------- */
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	lgl_compileShader(vertexShader, "shaders/vertex.glsl");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	lgl_compileShader(fragmentShader, "shaders/fragment.glsl");

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	lgl_linkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	xOffsetLocation = glGetUniformLocation(shaderProgram, "xOffset");

	/* VAO and VBO
	   ----------- */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
		     vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			      6 * sizeof(float), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			      6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	currentTime = glfwGetTime();
	previousTime = currentTime;

	while (!shouldQuit) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		timePassed += deltaTime;
		while (timePassed >= 2.0f) timePassed -= 2.0f;

		if (glfwWindowShouldClose(window)) shouldQuit = 1;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		xOffset = sin(timePassed * M_PI) / 2.0f;
		glUniform1f(xOffsetLocation, xOffset);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

 defer:
	if (glLoaded) {
		glDeleteProgram(shaderProgram);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	glfwTerminate();
	return exitCode;
}
