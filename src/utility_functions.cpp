#include "pch.h"
#include "utility_functions.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

GLuint CreateShader(GLenum shader_type, const char* shader_source) {
	GLuint ID = glCreateShader(shader_type);

	int src_len = strlen(shader_source);
	glShaderSource(ID, 1, &shader_source, &src_len);
	glCompileShader(ID);

	int info_log_length = 0;
	glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &info_log_length);

	if (info_log_length) {
		std::string info_log;
		info_log.resize(info_log_length);
		glGetShaderInfoLog(ID, info_log.size(), nullptr, const_cast<char*>(info_log.c_str()));
		printf("%s\n", info_log.c_str());
	}

	int compile_status = 0;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		glDeleteShader(ID);
		return 0;
	}

	return ID;
}

Program CreateProgram(std::map<GLenum, const char*>& shader_source_map) {

	const GLenum shader_types[5]{
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_GEOMETRY_SHADER,
		GL_TESS_CONTROL_SHADER,
		GL_TESS_EVALUATION_SHADER
	};

	Program p{ glCreateProgram() };

	for (int k = 0; k < 5; k++) {
		GLenum shader_type = shader_types[k];
		if (shader_source_map.count(shader_type)) {
			p.shaders[shader_type] = CreateShader(shader_type, shader_source_map[shader_type]);
			glAttachShader(p.program, p.shaders[shader_type]);
		}
	}

	glLinkProgram(p.program);

	int info_log_length = 0;
	glGetProgramiv(p.program, GL_INFO_LOG_LENGTH, &info_log_length);

	if (info_log_length) {
		std::string info_log;
		info_log.resize(info_log_length);
		glGetProgramInfoLog(p.program, info_log.size(), nullptr, const_cast<char*>(info_log.c_str()));
		printf("%s\n", info_log.c_str());
	}

	int link_status = 0;
	glGetProgramiv(p.program, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE) {
		DestroyProgram(p);
		return p;
	}

	glValidateProgram(p.program);

	glGetProgramiv(p.program, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length) {
		std::string info_log;
		info_log.resize(info_log_length);
		glGetProgramInfoLog(p.program, info_log.size(), nullptr, const_cast<char*>(info_log.c_str()));
		printf("%s\n", info_log.c_str());
	}

	int validate_status = 0;
	glGetProgramiv(p.program, GL_VALIDATE_STATUS, &validate_status);
	if (validate_status != GL_TRUE) {
		DestroyProgram(p);
		return p;
	}

	return p;
}

void Uniform1f(Program& program, const GLchar* name, GLfloat v0) { glUniform1f(glGetUniformLocation(program, name), v0); }
void Uniform2f(Program& program, const GLchar* name, GLfloat v0, GLfloat v1) { glUniform2f(glGetUniformLocation(program, name), v0, v1); }
void Uniform3f(Program& program, const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2) { glUniform3f(glGetUniformLocation(program, name), v0, v1, v2); }
void Uniform4f(Program& program, const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { glUniform4f(glGetUniformLocation(program, name), v0, v1, v2, v3); }
void Uniform1i(Program& program, const GLchar* name, GLint v0) { glUniform1i(glGetUniformLocation(program, name), v0); }
void Uniform2i(Program& program, const GLchar* name, GLint v0, GLint v1) { glUniform2i(glGetUniformLocation(program, name), v0, v1); }
void Uniform3i(Program& program, const GLchar* name, GLint v0, GLint v1, GLint v2) { glUniform3i(glGetUniformLocation(program, name), v0, v1, v2); }
void Uniform4i(Program& program, const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3) { glUniform4i(glGetUniformLocation(program, name), v0, v1, v2, v3); }
void Uniform1ui(Program& program, const GLchar* name, GLuint v0) { glUniform1ui(glGetUniformLocation(program, name), v0); }
void Uniform2ui(Program& program, const GLchar* name, GLuint v0, GLuint v1) { glUniform2ui(glGetUniformLocation(program, name), v0, v1); }
void Uniform3ui(Program& program, const GLchar* name, GLuint v0, GLuint v1, GLuint v2) { glUniform3ui(glGetUniformLocation(program, name), v0, v1, v2); }
void Uniform4ui(Program& program, const GLchar* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3) { glUniform4ui(glGetUniformLocation(program, name), v0, v1, v2, v3); }
void Uniform1fv(Program& program, const GLchar* name, GLsizei count, const GLfloat* value) { glUniform1fv(glGetUniformLocation(program, name), count, value); }
void Uniform2fv(Program& program, const GLchar* name, GLsizei count, const GLfloat* value) { glUniform2fv(glGetUniformLocation(program, name), count, value); }
void Uniform3fv(Program& program, const GLchar* name, GLsizei count, const GLfloat* value) { glUniform3fv(glGetUniformLocation(program, name), count, value); }
void Uniform4fv(Program& program, const GLchar* name, GLsizei count, const GLfloat* value) { glUniform4fv(glGetUniformLocation(program, name), count, value); }
void Uniform1iv(Program& program, const GLchar* name, GLsizei count, const GLint* value) { glUniform1iv(glGetUniformLocation(program, name), count, value); }
void Uniform2iv(Program& program, const GLchar* name, GLsizei count, const GLint* value) { glUniform2iv(glGetUniformLocation(program, name), count, value); }
void Uniform3iv(Program& program, const GLchar* name, GLsizei count, const GLint* value) { glUniform3iv(glGetUniformLocation(program, name), count, value); }
void Uniform4iv(Program& program, const GLchar* name, GLsizei count, const GLint* value) { glUniform4iv(glGetUniformLocation(program, name), count, value); }
void Uniform1uiv(Program& program, const GLchar* name, GLsizei count, const GLuint* value) { glUniform1uiv(glGetUniformLocation(program, name), count, value); }
void Uniform2uiv(Program& program, const GLchar* name, GLsizei count, const GLuint* value) { glUniform2uiv(glGetUniformLocation(program, name), count, value); }
void Uniform3uiv(Program& program, const GLchar* name, GLsizei count, const GLuint* value) { glUniform3uiv(glGetUniformLocation(program, name), count, value); }
void Uniform4uiv(Program& program, const GLchar* name, GLsizei count, const GLuint* value) { glUniform4uiv(glGetUniformLocation(program, name), count, value); }
void UniformMatrix2fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix2fv(glGetUniformLocation(program, name), count, transpose, value); }
void UniformMatrix3fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix3fv(glGetUniformLocation(program, name), count, transpose, value); }
void UniformMatrix4fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4fv(glGetUniformLocation(program, name), count, transpose, value); }
void UniformMatrix2x3fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix2x3fv(glGetUniformLocation(program, name), count, transpose, value); }
void UniformMatrix2x4fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix2x4fv(glGetUniformLocation(program, name), count, transpose, value); }
void UniformMatrix3x2fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix3x2fv(glGetUniformLocation(program, name), count, transpose, value); }
void UniformMatrix3x4fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix3x4fv(glGetUniformLocation(program, name), count, transpose, value); }
void UniformMatrix4x2fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4x2fv(glGetUniformLocation(program, name), count, transpose, value); }
void UniformMatrix4x3fv(Program& program, const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4x3fv(glGetUniformLocation(program, name), count, transpose, value); }

void DestroyProgram(Program& p) {
	for (std::pair<GLenum, GLuint> shader : p.shaders) {
		glDetachShader(p.program, shader.second);
		glDeleteShader(shader.second);
	}
	p.shaders.clear();
	if (p.program) glDeleteProgram(p.program);
	p = {};
}

VertexArray CreateVertexArray(const std::vector<VBO_info>& vbo_infos, const std::vector<attrib_info>& attrib_infos, GLsizei index_count, GLuint* indices) {

	VertexArray v = {};
	glGenVertexArrays(1, &v.VAO);
	glBindVertexArray(v);

	for (int k = 0; k < static_cast<int>(vbo_infos.size()); k++) {
		GLuint VBO = 0;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		if (vbo_infos[k].pointer) {
			glBufferData(GL_ARRAY_BUFFER, vbo_infos[k].count*vbo_infos[k].stride, vbo_infos[k].pointer, GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexBuffer(k, VBO, vbo_infos[k].offset, vbo_infos[k].stride);
		v.VBOs[vbo_infos[k].name] = VBO;

		v.count = vbo_infos[k].count;
	}

	for (int k = 0; k < static_cast<int>(attrib_infos.size()); k++) {
		glEnableVertexAttribArray(k);
		switch (attrib_infos[k].type) {
		case GL_FLOAT:
			glVertexAttribFormat(k, attrib_infos[k].channels, attrib_infos[k].type, GL_FALSE, attrib_infos[k].relative_offset);
			break;
		case GL_UNSIGNED_BYTE:
		case GL_UNSIGNED_INT:
		case GL_UNSIGNED_SHORT:
			glVertexAttribIFormat(k, attrib_infos[k].channels, attrib_infos[k].type, attrib_infos[k].relative_offset);
			break;
		case GL_DOUBLE:
			glVertexAttribLFormat(k, attrib_infos[k].channels, attrib_infos[k].type, attrib_infos[k].relative_offset);
			break;
		}
		glVertexAttribBinding(k, attrib_infos[k].binding_index);
	}

	if (indices) {
		glGenBuffers(1, &v.IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v.IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(GLuint), indices, GL_STATIC_DRAW);

		v.count = index_count;
	}

	glBindVertexArray(0);

	return v;
}

void VertexBufferData(VertexArray& vao, const char* buffer_name, GLsizei count, GLsizeiptr size, const GLvoid* data, GLenum usage) {
	glBindBuffer(GL_ARRAY_BUFFER, vao.VBOs[buffer_name]);
	glBufferData(GL_ARRAY_BUFFER, size*count, data, usage);
	vao.count = count;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IndexBufferData(VertexArray& vao, GLsizei count, const GLvoid* data, GLenum usage) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage);
	vao.count = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DestroyVertexArray(VertexArray& v) {
	for (std::pair<std::string, GLuint> VBO : v.VBOs) {
		glDeleteBuffers(1, &VBO.second);
	}
	v.VBOs.clear();
	if (v.IBO) glDeleteBuffers(1, &v.IBO);
	if (v.VAO) glDeleteVertexArrays(1, &v.VAO);
	v = {};
}

Texture2D CreateTexture2D(GLsizei width, GLsizei height, GLsizei levels, GLenum internal_format, GLenum format, GLenum type, const GLvoid* pixels) {
	Texture2D t = { 0, width, height, levels };
	glGenTextures(1, &t.TEX);

	glBindTexture(GL_TEXTURE_2D, t);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, levels ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);

	if (width != 0 && height != 0) {
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, levels ? (GetMipLevels(width, height) - 1) : 0);
		if (levels) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return t;
}

void TextureImage2D(Texture2D t, GLsizei width, GLsizei height, GLsizei level, GLenum internal_format, GLenum format, GLenum type, const GLvoid* pixels) {
	glBindTexture(GL_TEXTURE_2D, t);
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, 0, format, type, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GenerateMipmap(Framebuffer fbo, Texture2D texture, Program program, VertexArray vao, GLuint level) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glUseProgram(program);
	glBindVertexArray(vao);

	for (int i = 0; i < static_cast<int>(level); i++) {

		Uniform1i(program, "lod", i);
		Uniform1i(program, "tex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, i + 1);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glFinish();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(0);
	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DestroyTexture2D(Texture2D& t) {
	if (t.TEX) glDeleteTextures(1, &t.TEX);
	t = {};
}

Framebuffer CreateFramebuffer(GLsizei width, GLsizei height, bool useRenderbuffer) {
	Framebuffer f;
	glGenFramebuffers(1, &f.FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, f);

	if (useRenderbuffer) {
		glGenRenderbuffers(1, &f.renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, f.renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, f.renderbuffer);
	}

	f.rendertexture = CreateTexture2D(width, height, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, f.rendertexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return {};
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return f;
}

Framebuffer CreateFramebuffer(Texture2D t, bool useRenderbuffer) {
	Framebuffer f;
	glGenFramebuffers(1, &f.FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, f);

	if (useRenderbuffer) {
		glGenRenderbuffers(1, &f.renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, f.renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t.width, t.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, f.renderbuffer);
	}

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, t, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return {};
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return f;
}

void AttachFramebufferTexture(Framebuffer f, Texture2D t, GLsizei level) {
	glBindFramebuffer(GL_FRAMEBUFFER, f);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, t, level);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DestroyFramebuffer(Framebuffer& f) {
	if (f.rendertexture) glDeleteTextures(1, &f.rendertexture);
	if (f.renderbuffer) glDeleteRenderbuffers(1, &f.renderbuffer);
	if (f.FBO) glDeleteFramebuffers(1, &f.FBO);
	f = {};
}


/* https://stackoverflow.com/questions/2589096/find-most-significant-bit-left-most-that-is-set-in-a-bit-array */
uint32_t msbDeBruijn32(uint32_t v)
{
	static const int MultiplyDeBruijnBitPosition[32] =
	{
		0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
		8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
	};

	v |= v >> 1; // first round down to one less than a power of 2
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;

	return MultiplyDeBruijnBitPosition[(uint32_t)(v * 0x07C4ACDDU) >> 27];
}

GLsizei GetMipLevels(GLsizei width, GLsizei height) {
	return msbDeBruijn32(glm::max(width, height)) + 1;
}
