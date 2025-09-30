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

	unsigned int textures[2];
	int texWidth, texHeight, texNrChannels;
	unsigned char *texData;

	/* Please note that we changed our texture coords to "zoom in" */
	float vertices[] = {
		/* positions          colors              texture coords */
		0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   0.55f, 0.55f, /* top right */
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   0.55f, 0.45f, /* bottom right */
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f, /* bottom left */
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f  /* top left */
	};
	unsigned int indices[] = {
		0, 1, 3, /* first triangle */
		1, 2, 3  /* second triangle */
	};

	/* OpenGL texture coordinations
	 * 0.0 is bottom left and 1.1 is top right in texture coords
	 * This is why we need stb_image to flip images for us, or
	 * we can flip the coords in the shaders */
	stbi_set_flip_vertically_on_load(1);

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

	/* texture
	   ******* */
	glGenTextures(2, textures);

	/* container texture */
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

	/* awesomeface texture */
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	texData = stbi_load("assets/textures/awesomeface.png",
			    &texWidth, &texHeight, &texNrChannels, 0);

	if (texData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0,
			     GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texData);
	} else {
		fprintf(stderr, "Failed to load texture \"assets/textures/awesomeface.png\"");
	}


	/* bind uniforms */
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

	while (!shouldQuit) {
		if (glfwWindowShouldClose(window)) shouldQuit = 1;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

 defer:
	if (glLoaded) {
		glDeleteProgram(shaderProgram);
		glDeleteTextures(2, textures);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	glfwTerminate();
	return exitCode;
}
