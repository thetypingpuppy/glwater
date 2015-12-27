#version 430

void main(void)
{
gl_FrontColor = vec4(0.0, 1.0, 0.0, 1.0);
gl_Position = vec4(gl_Vertex[0],gl_Vertex[1] , 0.0f, 1.0f); // 2d only


gl_PointSize = 2;//*(1 - 0.5*(gl_Position[1]+1.0));

}
