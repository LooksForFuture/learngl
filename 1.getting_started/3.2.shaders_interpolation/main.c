#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stdio.h>

#define goto_defer(value) do { exitCode = (value); goto defer; } while(0)

const char *vertexShaderSource = "#version 330 core\n"
				 "layout (location = 0) in vec3 aPos;\n"
				 "layout (location = 1) in vec3 aColor;\n"
				 "out vec3 ourColor;\n"
				 "void main() {\n"
				 "gl_Position = vec4(aPos, 1.0f);\n"
				 "ourColor = aColor;\n"
				 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
				   "out vec4 FragColor;\n"
				   "in vec3 ourColor;\n"
				   "void main() {\n"
				   "FragColor = vec4(ourColor, 1.0f);"
				   "}\0";

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

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to create window!\n");
		goto_defer(-1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to load OpenGL!\n");
		goto_defer(-1);
	}
	glLoaded = 1;
	glfwSetFramebufferSizeCallback(window, onWindowResize);

	/* shader creation
	   --------------- */
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	{
		int success = 0;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			fprintf(stderr, "Vertex shader compilation failed!\n%s\n", infoLog);
		}
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	{
		int success = 0;
		char infoLog[512];

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			fprintf(stderr, "Fragment shader compilation failed!\n%s\n", infoLog);
		}
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	{
		int success = 0;
		char infoLog[512];

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			fprintf(stderr, "Program linking failed!\n%s\n", infoLog);
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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

	while (!shouldQuit) {
		if (glfwWindowShouldClose(window)) shouldQuit = 1;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

 defer:
	if (glLoaded) {
		glDeleteProgram(shaderProgram);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	glfwTerminate();
	return exitCode;
}
