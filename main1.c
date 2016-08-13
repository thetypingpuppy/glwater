#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_BUFFER_OBJECTS 10
#define NUM_PARTICLES 1024
#define WORK_GROUP_SIZE 128
#define GRID_X 80.0
#define GRID_Y 60.0

HGLRC ourOpenGLRenderingContext;
const char g_szClassName[] = "myWindowClass";
HDC ourWindowHandleToDeviceContext;
GLuint mProgram;
GLuint mProgram2;
GLuint mComputeProgram;
GLuint mDensityComputeProgram;
  GLuint temp[NUM_BUFFER_OBJECTS];
struct pos{
  float x,y,z,w;
};

struct vel{
  float vx,vy,vz,vw;
};

struct prop{
  float density, pressure, unused1, unused2;
};


// OpenGL Function Pointers
PFNGLGENBUFFERSPROC glGenBuffers; // These typedefs are provided in glext.h
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
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
PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;
PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
PFNGLMEMORYBARRIERPROC glMemoryBarrier;
//PFNGLCOLORPOINTER glColorPointer;

/* FUNCTIONS */

void glextraGetFunctionPointers(){

        //wglGetProcAddress is provided in windows.h
        glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
        glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
        glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)wglGetProcAddress("glBindBufferBase");
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
        glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)wglGetProcAddress("glBindBufferRange");
        glBeginTransformFeedback  = (PFNGLBEGINTRANSFORMFEEDBACKPROC)wglGetProcAddress("glBeginTransformFeedback");
        glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)wglGetProcAddress("glEndTransformFeedback");
        glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)wglGetProcAddress("glBindTransformFeedback");
        glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
        glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
        glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)wglGetProcAddress("glGenTransformFeedbacks");
        glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
        glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
        glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
        glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)wglGetProcAddress("glDispatchCompute");
        glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)wglGetProcAddress("glMemoryBarrier");
        //glColorPointer = (PFNGLCOLORPOINTER)wglGetProcAddress("glColorPointer");
}


GLuint glExtraCreateShaderFromSourceFile(GLenum shaderType, char* sourceFileWithExtension){

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

    GLint maxLength = 0;
  	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    GLchar logString[maxLength + 1];
    glGetShaderInfoLog(shader, maxLength, 0, logString);

    FILE *f = fopen("shaderInBuffer.txt", "w");
    fprintf(f,buffer);
    fclose(f);
    MessageBoxA(0,(char*)logString, "Shader Status",0);

  }

    free(buffer);
    return shader;

}

void draw(GLuint vbo[NUM_BUFFER_OBJECTS], double width, double height){


  // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, vbo[0]);
  // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[1]);



  glUseProgram(mComputeProgram);
  glDispatchCompute( NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1 );
//glFinish();
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

//


glViewport(0, 0, width, height);
glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glColor3f(0.0f, 0.0f, 0.0f);


//Craig is a twat





  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(
  //    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
  //    4,                  // size
  //    GL_FLOAT,           // type
  //    GL_FALSE,           // normalized?
  //    0*sizeof(GLfloat),                  // stride
  //    (void*)0            // array buffer offset
  // );

glEnableClientState(GL_VERTEX_ARRAY);

glUseProgram(mProgram);
glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  glVertexPointer( 4, GL_FLOAT, 0, (void *)0 );

  glDrawArrays(GL_POINTS, 0, NUM_PARTICLES); // Starting from vertex 0; 3 vertices total -> 1 triangles

  glBindBuffer(GL_ARRAY_BUFFER, 0);



  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  ///glFlush();

  glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
  glVertexPointer( 4, GL_FLOAT, 0, (void *)0 );


  glDrawArrays(GL_POINTS, 0, 300); // Starting from vertex 0; 3 vertices total -> 1 triangles

  glBindBuffer(GL_ARRAY_BUFFER, 0);


//  glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
  //glViewport(0, 0, width, height);



  glUseProgram(mProgram2);


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );






      glBindBuffer(GL_ARRAY_BUFFER, vbo[7]); // GRID
        glVertexPointer( 4, GL_FLOAT, 0, (void *)0 );
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[8]); // GRID Color
          glColorPointer( 4, GL_FLOAT, 0, (void *)0 );


          glEnableClientState(GL_COLOR_ARRAY);
  //glEnableVertexAttribArray(0);


  glDrawArrays(GL_POINTS, 0, (GLsizei)GRID_X*GRID_Y);

glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

    glDisable( GL_BLEND );

//  glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

for(int i=0;i<NUM_BUFFER_OBJECTS;i++){

  temp[i] = vbo[i];

}

vbo[0] = temp[3];
vbo[1] = temp[4];
vbo[2] = temp[5];
vbo[3] = temp[0];
vbo[4] = temp[1];
vbo[5] = temp[2];


glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo[1]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vbo[3]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, vbo[4]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, vbo[7]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, vbo[8]);




//glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[7]);





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
    //GLfloat particlesXYZ[900];
    GLuint vbo[NUM_BUFFER_OBJECTS];

    GLuint vertexShader;

    double width;
    double height;


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
         ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
         wglMakeCurrent (ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);


         MessageBoxA(0,(char*)glGetString(GL_VERSION), "OPENGL VERSION",0);

      glextraGetFunctionPointers();


  // Set up buffer objects
 glGenBuffers(NUM_BUFFER_OBJECTS, vbo);

// SET UP COMPUTE
glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[0]);
glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES*sizeof(struct pos), NULL, GL_STATIC_DRAW);
GLint bufMask = GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT;
struct pos *points = (struct pos *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct pos), bufMask);

for(int i=0; i<NUM_PARTICLES; i++){

  points[i].x =  -0.8f + 1.6f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX);

  points[i].y =  -0.8f + 1.6f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX);
  points[i].z =   0.0f;
  points[i].w = 0.0f;

}

glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[1]);
glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct vel), NULL, GL_STATIC_DRAW);

struct vel *vels= (struct vel*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct vel), bufMask);


for(int i=0; i<NUM_PARTICLES; i++){

  vels[i].vx = 0.0 *( -1.0f + 2.0f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX));

  vels[i].vy = 0.0 *( -1.0f + 2.0f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX));
  vels[i].vz =   0.0f;
  vels[i].vw = 0.0f;

}

glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[2]);
glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct prop), NULL, GL_STATIC_DRAW);

struct prop *props = (struct prop*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct prop), bufMask);

for(int i=0; i<NUM_PARTICLES; i++){

  props[i].density =  (GLfloat) 100.0f;// * (GLfloat) (rand()) / (GLfloat) (RAND_MAX);

  props[i].pressure =  0.0f;// * (GLfloat) (rand()) / (GLfloat) (RAND_MAX);
  props[i].unused1 =   0.0f;
  props[i].unused2 =   0.0f;

}

glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[3]);
glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct pos), NULL, GL_STATIC_DRAW);

struct pos *points2 = (struct pos *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct pos), bufMask);

for(int i=0; i<NUM_PARTICLES; i++){

  points2[i].x =  -0.2f + 0.4f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX);

  points2[i].y =  -0.8f + 1.6f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX);
  points2[i].z =   0.0f;
  points2[i].w = 0.0f;

}



glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[4]);
glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct vel), NULL, GL_STATIC_DRAW);

struct vel *vels2= (struct vel*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct vel), bufMask);


for(int i=0; i<NUM_PARTICLES; i++){

  vels2[i].vx =  0.1f *( -1.0f + 2.0f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX));

  vels2[i].vy =  0.1f *( -1.0f + 2.0f * (GLfloat) (rand()) / (GLfloat) (RAND_MAX));
  vels2[i].vz =   0;
  vels2[i].vw = 0;

}

glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[5]);
glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct prop), NULL, GL_STATIC_DRAW);

struct prop *props2 = (struct prop*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct prop), bufMask);

for(int i=0; i<NUM_PARTICLES; i++){

  props2[i].density = (GLfloat) 100.0f;// * (GLfloat) (rand()) / (GLfloat) (RAND_MAX);

  props2[i].pressure =  0.0f ;//* (GLfloat) (rand()) / (GLfloat) (RAND_MAX);
  props2[i].unused1 =   0;
  props2[i].unused2 =   0;

}

glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[6]);
glBufferData(GL_SHADER_STORAGE_BUFFER, 300 * sizeof(struct pos), NULL, GL_STATIC_DRAW);

struct pos *boundaryPoints= (struct pos*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 300 * sizeof(struct prop), bufMask);

float x = -0.9;
float y = 0.9;
for(int i=0; i<300; i++){

  if(i<100){


    boundaryPoints[i].x = -0.9;
    boundaryPoints[i].y = y;
    y -= 0.02;
  }

  if((i<201) & (i >= 100)){

    boundaryPoints[i].x = x;
    boundaryPoints[i].y = -0.9;
    x += 0.02;

  }

  if((i<300) & (i >= 201)){

    boundaryPoints[i].x = 1.0;
    boundaryPoints[i].y = y;
    y += 0.02;

  }

  boundaryPoints[i].z =   0.0f;
  boundaryPoints[i].w = 0.0f;


}

glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[7]);
glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)GRID_X*GRID_Y * sizeof(struct pos), NULL, GL_STATIC_DRAW);

struct pos *grid = (struct pos *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (GLsizeiptr)GRID_X*GRID_Y * sizeof(struct pos), bufMask);


for (int j=0; j<GRID_Y; j++){
  for(int i=0; i<GRID_X; i++){

      grid[j*(int)GRID_X+i].x = -1.0f  + 1.0f/GRID_X + (GLfloat)i*(2-(1/(GLfloat)GRID_X))/((GLfloat)GRID_X);
      grid[j*(int)GRID_X+i].y = -1.0f  + 1.0f/GRID_Y + (GLfloat)j*(2-(1/GRID_Y))/(GRID_Y);;
      grid[j*(int)GRID_X+i].z = 0.0f;
      grid[j*(int)GRID_X+i].w = 1.0f;
  }
}
glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[8]);
glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)GRID_X*GRID_Y * sizeof(struct pos), NULL, GL_STATIC_DRAW);

struct pos *gridColors = (struct pos *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (GLsizeiptr)GRID_X*GRID_Y * sizeof(struct pos), bufMask);


for (int j=0; j<GRID_Y; j++){
  for(int i=0; i<GRID_X; i++){

      //RED
      gridColors[j*(int)GRID_X+i].x = 1.0f;//-1.0f  + 1.0f/GRID_X + (GLfloat)i*(2-(1/(GLfloat)GRID_X))/((GLfloat)GRID_X);
      //GREEN
      gridColors[j*(int)GRID_X+i].y = 0.0f;//  + 1.0f/GRID_Y + (GLfloat)j*(2-(1/GRID_Y))/(GRID_Y);;
      //BLUE
      gridColors[j*(int)GRID_X+i].z = 0.0f;
      //ALPHA
      gridColors[j*(int)GRID_X+i].w = 1.0f;
  }
}
glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[9]);
glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)GRID_X*GRID_Y * sizeof(struct pos), NULL, GL_STATIC_DRAW);

struct pos *gridActivations = (struct pos *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (GLsizeiptr)GRID_X*GRID_Y * sizeof(struct pos), bufMask);


for (int j=0; j<GRID_Y; j++){
  for(int i=0; i<GRID_X; i++){

      gridActivations[j*(int)GRID_X+i].x = 0.0f;//-1.0f  + 1.0f/GRID_X + (GLfloat)i*(2-(1/(GLfloat)GRID_X))/((GLfloat)GRID_X);
       gridActivations[j*(int)GRID_X+i].y = 0.0f;//  + 1.0f/GRID_Y + (GLfloat)j*(2-(1/GRID_Y))/(GRID_Y);;
       gridActivations[j*(int)GRID_X+i].z = 0.0f;
       gridActivations[j*(int)GRID_X+i].w = 0.0f;
  }
}
glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo[1]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vbo[3]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, vbo[4]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);

glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, vbo[7]);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, vbo[8]);

GLuint computeShader   = glExtraCreateShaderFromSourceFile(GL_COMPUTE_SHADER,   "computeshader.glsl");



mComputeProgram = glCreateProgram();
glAttachShader(mComputeProgram, computeShader);
glLinkProgram(mComputeProgram);

GLint isLinked = 0;
glGetProgramiv(mComputeProgram, GL_LINK_STATUS, (int *)&isLinked);
if(isLinked == GL_FALSE){

  MessageBoxA(0,"Failed to link OpenGl Compute program", "OpenGL Compute Program Status",0);
}

GLuint densityComputeShader   = glExtraCreateShaderFromSourceFile(GL_COMPUTE_SHADER,   "densityshader.glsl");
mDensityComputeProgram = glCreateProgram();

glAttachShader(mDensityComputeProgram, densityComputeShader);
glLinkProgram(mDensityComputeProgram);




      vertexShader   = glExtraCreateShaderFromSourceFile(GL_VERTEX_SHADER,   "vertexshader.glsl");
      GLuint fragmentShader = glExtraCreateShaderFromSourceFile(GL_FRAGMENT_SHADER, "fragmentshader.glsl");
      mProgram = glCreateProgram();
      glEnable(GL_PROGRAM_POINT_SIZE);
      glAttachShader(mProgram, vertexShader);
      glAttachShader(mProgram, fragmentShader);
      glLinkProgram(mProgram);
      isLinked = 0;
      glGetProgramiv(mProgram, GL_LINK_STATUS, (int *)&isLinked);
      if(isLinked == GL_FALSE){

        MessageBoxA(0,"Failed to link OpenGl program", "OpenGL Program Status",0);
      }

      GLuint vertexShaderBlobs   = glExtraCreateShaderFromSourceFile(GL_VERTEX_SHADER,   "MetablobVertexShader.glsl");
      mProgram2 = glCreateProgram();
    //  glEnable(GL_PROGRAM_POINT_SIZE);

//

      //glEnable(GL_POINT_SMOOTH);
    //  glEnable(GL_POINT_SIZE);
      glAttachShader(mProgram2, vertexShaderBlobs);
      glAttachShader(mProgram2, fragmentShader);

      glLinkProgram(mProgram2);
      isLinked = 0;
      glGetProgramiv(mProgram2, GL_LINK_STATUS, (int *)&isLinked);
      if(isLinked == GL_FALSE){

        MessageBoxA(0,"Failed to link OpenGl program 2", "OpenGL Program Status",0);
      }

    glUseProgram(mDensityComputeProgram);
    glDispatchCompute( NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1 );
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

      glUseProgram(mProgram);

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


              draw(vbo, width, height);
          }
      }

       return Msg.wParam;
}
