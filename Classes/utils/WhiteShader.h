#ifndef WHITE_SHADER_H_
#define WHITE_SHADER_H_

#include "cocos2d.h"

USING_NS_CC;

const static char* kVSertexShader = "attribute vec4 a_position;"\
"attribute vec2 a_texCoord;"\
"attribute vec4 a_color;"\
""\
"\n#ifdef GL_ES\n"\
"varying lowp vec4 v_fragmentColor;"\
"varying mediump vec2 v_texCoord;"\
"\n#else\n"\
"varying vec4 v_fragmentColor;"\
"varying vec2 v_texCoord;"\
"\n#endif\n"\
""\
"void main()"\
"{"\
"    gl_Position = CC_PMatrix * a_position;"\
"    v_fragmentColor = a_color;"\
"    v_texCoord = a_texCoord;"\
"}";

const static char* kFragmetShader = ""\
"\n#ifdef GL_ES\n"\
"precision lowp float;"\
"\n#endif\n"\
"varying vec4 v_fragmentColor;"\
"varying vec2 v_texCoord;"\
"uniform float t; "\
"void main()"\
"{"\
"vec4 color = texture2D(CC_Texture0, v_texCoord);"\
"gl_FragColor = color.a * mix(color, vec4(1.0,1.0,1.0,1.0), t);"\
"}";

static GLProgram* getWhiteShader()
{
	GLProgram * p = new GLProgram();
	p->initWithByteArrays(kVSertexShader, kFragmetShader);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	p->link();
	p->updateUniforms();
	return p;
}

#endif /* WHITE_SHADER_H_ */