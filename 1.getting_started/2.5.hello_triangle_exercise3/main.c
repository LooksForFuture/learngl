#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#define goto_defer(value) do { exitCode = (value); goto defer; } while (0)

const char *vertexShaderSource = "#version 330 core\n"
				 "layout (location = 0) in vec3 aPos;\n"
				 "void main() {\n"
				 "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
				 "}\0";

const char *fragmentShader1Source = "#version 330 core\n"
				    "out vec4 FragColor;\n"
				    "void main() {\n"
				    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
				    "}\0";

const char *fragmentShader2Source = "#version 330 core\n"
				    "out vec4 FragColor;\n"
				    "void main() {\n"
				    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
				    "}\0";

void onWindowResize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	int exitCode = 0, glLoaded = 0, shouldQuit = 0;
	GLFWwindow *window = NULL;

	/* vertices of two triangles */
	float vertices1[] = {
		-1.0f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	float vertices2[] = {
		0.0f, -0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	unsigned int VAO[2], VBO[2];
	unsigned int vertexShader, fragmentShader1, fragmentShader2;
	unsigned int shaderProgram1, shaderProgram2;

	/* window creation */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to create window\n");
		goto_defer(-1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to load OpenGL\n");
		goto_defer(-1);
	}
	glLoaded = 1;
	glfwSetFramebufferSizeCallback(window, onWindowResize);

	/* shader creation */
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

	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
	glCompileShader(fragmentShader1);

	{
		int success = 0;
		char infoLog[512];
		glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
			fprintf(stderr, "Fragment shader 1 compilation failed!\n%s\n", infoLog);
		}
	}

	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
	glCompileShader(fragmentShader2);

	{
		int success = 0;
		char infoLog[512];
		glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
			fprintf(stderr, "Fragment shader 2 compilation failed!\n%s\n", infoLog);
		}
	}

	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);

	{
		int success = 0;
		char infoLog[512];
		glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
			fprintf(stderr, "Program 1 linking failed!\n%s\n", infoLog);
		}
	}

	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	{
		int success = 0;
		char infoLog[512];
		glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
			fprintf(stderr, "Program 2 linking failed!\n%s\n", infoLog);
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);

	/* VAO and VBO */
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]); /* first triangle */

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1),
		     vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			      3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]); /* second triangle */

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2),
		     vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			      3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	while (!shouldQuit) {
		if (glfwWindowShouldClose(window)) shouldQuit = 1;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram1);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

 defer:
	if (glLoaded) {
		glDeleteBuffers(2, VBO);
		glDeleteVertexArrays(2, VAO);
		glDeleteProgram(shaderProgram1);
		glDeleteProgram(shaderProgram2);
	}

	glfwTerminate();
	
	return exitCode;
}
