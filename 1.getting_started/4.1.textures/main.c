#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define LGL_SHADER_IMPLEMENTATION
#include <lgl_shader.h>

#include <stdio.h>

#define goto_defer(value) do { exitCode = (value); goto defer; } while (0)

void onWindowResize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	int exitCode = 0, glLoaded = 0, shouldQuit = 0;
	GLFWwindow *window = NULL;

	unsigned int vertexShader, fragmentShader, shaderProgram;
	unsigned int VAO, VBO, EBO;

	unsigned int texture;
	int texWidth, texHeight, texNrChannels;
	unsigned char *texData;

	float vertices[] = {
		/* positions          colors              texture coords */
		0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, /* top right */
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, /* bottom right */
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, /* bottom left */
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  /* top left */
	};
	unsigned int indices[] = {
		0, 1, 3, /* first triangle */
		1, 2, 3  /* second triangle */
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
		fprintf(stderr, "Failed to load OpenGL\n");
		goto_defer(-1);
	}
	glLoaded = 1;
	glfwSetFramebufferSizeCallback(window, onWindowResize);

	/* shaders */
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

	/* VAO and buffers */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
		     vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		     indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			      8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			      8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			      8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/* texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texData = stbi_load("assets/textures/container.jpg",
			    &texWidth, &texHeight, &texNrChannels, 0);

	if (texData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0,
			     GL_RGB, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texData);
	} else {
		fprintf(stderr, "Failed to load texture \"assets/textures/container.jpg\"");
	}

	/* while Joey De Vries did not write about texture units in this part
	 * of tutorial, I thought it's better to do so, because it can prevent
	 * some possible bugs with some drivers */
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

	while (!shouldQuit) {
		if (glfwWindowShouldClose(window)) shouldQuit = 1;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

 defer:
	if (glLoaded) {
		glDeleteProgram(shaderProgram);
		glDeleteTextures(1, &texture);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	glfwTerminate();
	return exitCode;
}
