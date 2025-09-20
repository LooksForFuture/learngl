#ifndef __LGL_SHADER__
#define __LGL_SHADER__

#include <stdio.h>
#include <stdlib.h>

char *lgl_readFile(const char *path);

void lgl_compileShader(unsigned int shader, const char *sourceFile);

void lgl_linkProgram(unsigned int program);

#endif /*__LGL_SHADER__*/

#ifdef LGL_SHADER_IMPLEMENTATION

char *lgl_readFile(const char *path)
{
	char *content = NULL;
	FILE *fptr;
	long size = 0;

	fptr = fopen(path, "r");
	if (!fptr) return NULL;

	if (fseek(fptr, 0, SEEK_END) < 0) return NULL;
	size = ftell(fptr);
	if (size < 0) return NULL;
	fseek(fptr, 0, SEEK_SET);

	content = malloc(size + 1);
	fread(content, size, 1, fptr);
	if (ferror(fptr)) {
		free(content);
		return NULL;
	}

	content[size] = 0;
	return content;
}

void lgl_compileShader(unsigned int shader, const char *sourceFile)
{
	const char *source = lgl_readFile(sourceFile);
	if (!source) {
		fprintf(stderr, "Could not read shader file \"%s\"\n", sourceFile);
		return;
	}

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	{
		int success = 0;
		char infoLog[512];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			fprintf(stderr, "Shader \"%s\" compilation failed!\nError: %s\n", sourceFile, infoLog);
		}
	}
}

void lgl_linkProgram(unsigned int program)
{
	glLinkProgram(program);

	{
		int success = 0;
		char infoLog[512];

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			fprintf(stderr, "Program linking failed!\nError: %s\n", infoLog);
		}
	}
}

#endif /*LGL_SHADER_IMPLEMENTATION*/
