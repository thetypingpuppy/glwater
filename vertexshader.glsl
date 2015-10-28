#version 420
out vec3 something;

void main(void)

{
gl_Position = vec4(gl_Vertex[0], gl_Vertex[1], 0.0f, 1.0f); // 2d only
something = vec3(gl_Vertex[0], gl_Vertex[1] - 0.00001f, 0.0f);
//gl_Position[1] -= 0.0000001;
gl_PointSize = abs(10*gl_Vertex[1]);
}
