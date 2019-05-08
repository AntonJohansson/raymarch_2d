#pragma once

#include <glad/glad.h>

//typedef struct shaderprogram {
//	GLuint handle, vertex_shader, fragment_shader;
//	bool compiled;
//} shaderprogram;

extern GLuint program_begin();
extern void program_end();
extern void program_link();

extern void program_attach_shader_from_file(const char* file, GLenum type);
extern void program_attach_shader_from_buffer(const char* buffer, GLenum type);
extern void program_bind_attribute(GLuint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLsizei offset);
