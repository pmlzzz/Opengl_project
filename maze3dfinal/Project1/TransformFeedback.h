/*
 *		Written by Tim McGraw, 2010
 *		www.csee.wvu.edu/~tmcgraw
 */

#ifndef __TRANSFORMFEEDBACK_H__
#define __TRANSFORMFEEDBACK_H__

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

extern PFNGLCREATESHADERPROC            glCreateShader          = NULL;
extern PFNGLATTACHSHADERPROC            glAttachShader          = NULL;
extern PFNGLSHADERSOURCEPROC            glShaderSource          = NULL;
extern PFNGLGETSHADERIVPROC             glGetShaderiv           = NULL;
extern PFNGLCOMPILESHADERPROC           glCompileShader         = NULL;
extern PFNGLDETACHSHADERPROC            glDetachShader          = NULL;
extern PFNGLDELETESHADERPROC            glDeleteShader          = NULL;
extern PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog      = NULL; 

extern PFNGLCREATEPROGRAMPROC           glCreateProgram         = NULL;
extern PFNGLLINKPROGRAMPROC             glLinkProgram           = NULL;
extern PFNGLGETPROGRAMIVPROC            glGetProgramiv          = NULL;
extern PFNGLUSEPROGRAMPROC              glUseProgram            = NULL;
extern PFNGLDELETEPROGRAMPROC           glDeleteProgram         = NULL;
extern PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog     = NULL; 
extern PFNGLPROGRAMPARAMETERIEXTPROC	glProgramParameteri		= NULL;
extern PFNGLGETUNIFORMLOCATIONPROC	   glGetUniformLocation    = NULL;
extern PFNGLUNIFORM1FPROC	            glUniform1f             = NULL;

extern PFNGLGENBUFFERSPROC              glGenBuffers            = NULL;
extern PFNGLBINDBUFFERPROC              glBindBuffer            = NULL;
extern PFNGLBUFFERDATAPROC              glBufferData            = NULL;

extern PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation          = NULL;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray    = NULL;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray  = NULL;
extern PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer        = NULL;

extern PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback		= NULL;
extern PFNGLENDTRANSFORMFEEDBACKPROC   glEndTransformFeedback		= NULL;
extern PFNGLBINDBUFFERBASEPROC		   glBindBufferBase				= NULL;
extern PFNGLBINDBUFFERRANGEPROC		   glBindBufferRange			= NULL;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings = NULL;

void glError();
void* wglLoadExtension(char* name);
void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);
void ExitGlut();

#endif