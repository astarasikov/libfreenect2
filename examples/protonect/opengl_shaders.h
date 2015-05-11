#ifndef __OPENGL_SHADERS__H__
#define __OPENGL_SHADERS__H__

#import "osx_gl_common.h"

#define QUOTE(A) #A

const char * const FRAG = "#version 150 core\n" QUOTE(
	in vec3 vert_color;
	in vec2 vert_texcoord;
	out vec4 out_color;
	uniform sampler2D texture_RGB;
	uniform sampler2D texture_DEPTH;
	uniform sampler2D texture_IR;

	void main(void) {
		//vec3 rgb = texture(texture_RGB, vert_texcoord).rgb;
		//float gray = dot(vec3(0.3), rgb);
		float depth = texture(texture_DEPTH, vert_texcoord).r / 4500.0;
		float ir = texture(texture_IR, vert_texcoord).r / 20000.0;
		
		//vec3 gvec = 0.5 * vec3(gray);
		//vec3 id = vec3(0, 0.5, 0.5) * vec3(0, ir, depth);
		//out_color = vec4(id + gvec, 1.0);
		//out_color = vec4(rgb, 1.0);
		out_color = vec4(vec3(depth), 1.0);
		//out_color = vec4(vec3(ir), 1.0);
	}
);

const char * const VERT = "#version 150 core\n" QUOTE(
	in vec4 position;
	in vec3 color;
	in vec2 texcoord;
	out vec3 vert_color;
	out vec2 vert_texcoord;

	void main(void) {
		gl_Position = position;
		vert_color = color;
		vert_texcoord = texcoord;
	}
);

#undef QUOTE

static inline void oglShaderLog(int sid) {
	GLint logLen;
	GLsizei realLen;

	glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &logLen);
	if (!logLen) {
		return;
	}
	char* log = (char*)malloc(logLen);
	if (!log) {
		NSLog(@"Failed to allocate memory for the shader log");
		return;
	}
	glGetShaderInfoLog(sid, logLen, &realLen, log);
	NSLog(@"shader %d log %s", sid, log);
	free(log);
}

#endif //__OPENGL_SHADERS__H__
