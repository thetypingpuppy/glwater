#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>

/* DECLARATIONS */

const char g_szClassName[] = "myWindowClass";
HDC ourWindowHandleToDeviceContext;
GLfloat particles[300];
double width;
double height;
GLuint vbo;

FILE *ptrToVertexShaderFile;
FILE *ptrToFragmentShaderFile;

// OpenGL Function Pointers
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

/* FUNCTIONS */

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

void draw(){


  glViewport(0, 0, width, height);
  glClearColor( 1.0f, 0.4f, 0.0f, 0.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(0.0f, 0.0f, 0.0f);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
     0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
     3,                  // size
     GL_FLOAT,           // type
     GL_FALSE,           // normalized?
     0,                  // stride
     (void*)0            // array buffer offset
  );

  // Draw the triangle !
//  glPointSize(5.0);

  glDrawArrays(GL_POINTS, 0, 100); // Starting from vertex 0; 3 vertices total -> 1 triangle
  glDisableVertexAttribArray(0);

  //RenderFrame();
  SwapBuffers(ourWindowHandleToDeviceContext);
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
      case WM_CREATE:
      {
      }
      break;
      case WM_CLOSE:
          DestroyWindow(hwnd);
      break;
      case WM_DESTROY:
          PostQuitMessage(0);
      break;
      default:
          return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* MAIN */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }



    RECT rect;
    SetRect( &rect, 50,  // left
                    50,  // top
                    850, // right
                    650 ); // bottom

    // Save width and height off.
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    // Adjust it.
    AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, 0 );


    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "glWater",
        WS_OVERLAPPEDWINDOW,
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }



    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    PIXELFORMATDESCRIPTOR pfd =
        {
          sizeof(PIXELFORMATDESCRIPTOR),
          1,
          PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
          PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
          32,                        //Colordepth of the framebuffer.
          0, 0, 0, 0, 0, 0,
          0,
          0,
          0,
          0, 0, 0, 0,
          24,                        //Number of bits for the depthbuffer
          8,                        //Number of bits for the stencilbuffer
          0,                        //Number of Aux buffers in the framebuffer.
          PFD_MAIN_PLANE,
          0,
          0, 0, 0
        };

        ourWindowHandleToDeviceContext = GetDC(hwnd);

        int  letWindowsChooseThisPixelFormat;
        letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
         SetPixelFormat(ourWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);
        //
         HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
         wglMakeCurrent (ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);


         MessageBoxA(0,(char*)glGetString(GL_VERSION), "OPENGL VERSION",0);

      glextraGetFunctionPointers();


      GLuint vertexShader   = glextraLoadShaderFromSourceFile(GL_VERTEX_SHADER,   "vertexshader.glsl");
      GLuint fragmentShader = glextraLoadShaderFromSourceFile(GL_FRAGMENT_SHADER, "fragmentShader.glsl");
      GLuint mProgram = glCreateProgram();
      glEnable(GL_PROGRAM_POINT_SIZE);
    //  glEnable(GL_POINT_SIZE);
      glAttachShader(mProgram, vertexShader);
      glAttachShader(mProgram, fragmentShader);

//      const GLchar* feedbackVaryings[] = { "outValue" };
  //    glTransformFeedbackVaryings(mProgram, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

      glLinkProgram(mProgram);


      GLint isLinked = 0;
      glGetProgramiv(mProgram, GL_LINK_STATUS, (int *)&isLinked);
      if(isLinked == GL_FALSE){

        MessageBoxA(0,"Failed to link OpenGl program", "OpenGL Program Status",0);
      }


      glUseProgram(mProgram);

//lol = 7;

      //glEnableVertexAttribArray

      glGenBuffers(1, &vbo);
     	glBindBuffer(GL_ARRAY_BUFFER, vbo);
      //glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      for(int i=0;i<300;i++){
            *(particles+i) = -1.0f + 2.0f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX);
       }

     	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);

      while( 1 )
      {
          if( PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) )
          {
              if( Msg.message == WM_QUIT )
              {
                  break;
              }

              TranslateMessage( &Msg );
              DispatchMessage( &Msg );
          }
          else
          {
              //6.  DRAW USING OPENGL.
              // This region right here is the
              // heart of our application.  THE MOST
              // execution time is spent just repeating
              // this draw() function.


              draw();
          }
      }

       return Msg.wParam;

}
