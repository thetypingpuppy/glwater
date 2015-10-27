void main(void)
{
gl_Position = vec4(gl_Vertex[0], gl_Vertex[1], 0.0f, 1.0f); // 2d only
gl_PointSize = abs(10*gl_Vertex[1]);
}
