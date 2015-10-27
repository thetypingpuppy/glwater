 #include <GL/gl.h>
 #include <GL/glu.h>
 #include <GL/glext.h>
 #include <stdio.h>


PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;


void glextraGetFunctionPointers();

GLuint glextraLoadShaderFromSourceFile(GLenum shaderType, char* sourceFileWithExtension);
