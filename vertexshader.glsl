#version 420

//in vec4 gl_Vertex
in vec2 densityAndPressure;
in vec3 vxvyvz;

out vec4 fbXYZw;
out vec2 fbDensityAndPressure;
out vec3 fbVxvyvz;


float particleMass = 0.1;
float h = 0.1; // smoothing parameter
float const1 = particleMass * 315 / 64 * 3.14 * pow(h,9);
float total;

float restingDensity = 1000;
float k = 0.5;
float density;
float pressure;
float pressureTerm;
float distances[100];
float locationsCopy[300];
int index;

void main(void)
{

//DISTANCES

locationsCopy[4*gl_VertexID] = gl_Vertex[0];
locationsCopy[4*gl_VertexID + 1] = gl_Vertex[1];
locationsCopy[4*gl_VertexID + 1] = gl_Vertex[2];

for (int i = 0; i<gl_VertexID;i++){

index = i * 4;
distances[index] = distance(vec3(gl_Vertex[0], gl_Vertex[1], gl_Vertex[2]),
                            vec3(locationsCopy[index], locationsCopy[index+1], locationsCopy[index+2]));

}

for(int i = gl_VertexID; i<100; i++){

  index = i * 4;
  distances[index] = distance(vec3(gl_Vertex[0], gl_Vertex[1], gl_Vertex[2]),
                                vec3(fbXYZw[index], fbXYZw[index+1], fbXYZw[index+2]));

}


// UPDATE DENSITY
total = 0;

for(int i = 0; i<100; i++){

  index = i * 4;

  total+= pow(pow(h,2) - pow(distances[index],2),3);

}

density = const1*total;
pressure = k*(density-restingDensity);


//something2[0] = total*const1;
//pressure = k * (something2[0] - restingDensity);

total = 0;

for(int i = 0; i<100; i++){

index = i * 4;

//pressureTerm

}



//if (gl_VertexID == 99){
fbXYZw = vec4(gl_Vertex[0], gl_Vertex[1] - 0.0001f, 0.0f, 1.0f);
fbDensityAndPressure = vec2(0.0f,0.0f);
fbVxvyvz = vec3(0.0f,0.0f,0.0f);
//}

//particlesXYZfb = vec4(gl_Vertex[0], gl_Vertex[1] - 0.0001f, 0.0f, 1.0f); // 1 means point in homogenous coords
//density = densityAndVelocity
//something2 = vec4(gl_Vertex[0], gl_Vertex[1] - 0.0001f, 0.0f, 1.0f);

gl_Position = vec4(gl_Vertex[0], sin(fbXYZw[1]), 0.0f, 1.0f); // 2d only

//gl_Position[1] -= 0.0000001;
gl_PointSize = abs(10*gl_Position[1]);

}
