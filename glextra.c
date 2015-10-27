//#include <windows.h>
#include <stdlib.h>
#include "glextra.h"

GLuint glextraLoadShaderFromSourceFile(GLenum shaderType, char* sourceFileWithExtension){



  FILE* ptrToShaderFile = fopen(sourceFileWithExtension,"r"); // pointer to the file containing the code
  GLchar * buffer = 0; // buffer where the characters will be stored
  GLuint shader = glCreateShader(shaderType); // the shader



  if (ptrToShaderFile)
  {
    long length;
    fseek (ptrToShaderFile, 0, SEEK_END);
    length = ftell (ptrToShaderFile);
    fseek (ptrToShaderFile, 0, SEEK_SET);
    buffer = (char*)calloc (1, length); // the first argument to calloc is 1 because a char is 1 byte
    if (buffer)
    {
      fread (buffer, 1, length, ptrToShaderFile);

    }
    fclose (ptrToShaderFile);
  }

  glShaderSource(shader,1,(const char* const*)&buffer,NULL);
  glCompileShader(shader);

  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {

    FILE *f = fopen("shaderInBuffer.txt", "w");
    fprintf(f,buffer);
    fclose(f);
    MessageBoxA(0,"Shader failed to compile", "Shader Status",0);

  }

    free(buffer);
    return shader;

}

void glextraGetFunctionPointers(){

        glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
        glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
        glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
        glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
        glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
        glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
        glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
        glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
        glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
        glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
        glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
        glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
        glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
        glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
        glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
        glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)wglGetProcAddress("glTransformFeedbackVaryings");
}
