#version 430

void main(void)
{
gl_FrontColor = vec4(0.0, 0.2, 1.0, gl_Vertex[3]);
gl_Position = gl_Vertex;//vec4(gl_Vertex[0],gl_Vertex[1] , 0.0f, 1.0f);

//vec4(gl_Vertex[0],gl_Vertex[1] , 0.0f, 1.0f); // 2d only


gl_PointSize = 1;//*(1 - 0.5*(gl_Position[1]+1.0));

}
