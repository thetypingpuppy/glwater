
#version 430
#define h 0.2
#define particleMass 0.4
#define k 1000
#define restingDensity 100.0
#define absoluteViscosity 0.01
#define timeStep 0.000000000000000000000000004
#define gravity 0//-9.8
//typedef PropertiesPrevious[gid].x PropertiesPrevious[gid].density

layout( std140, binding=0 ) buffer PosPrev
{
vec4 PositionsPrevious[ ]; // array of structures
};

layout( std140, binding=1 ) buffer VelPrev
{
vec4 VelocitiesPrevious[ ]; // array of structures
};

layout( std140, binding=2 ) buffer PropPrev
{
vec4 PropertiesPrevious[ ]; // array of structures
};

layout( std140, binding=3 ) buffer Pos2
{
vec4 Positions[ ]; // array of structures
};

layout( std140, binding=4 ) buffer Vel2
{
vec4 Velocities[ ]; // array of structures
};

layout( std140, binding=5 ) buffer Prop2
{
vec4 Properties[ ]; // array of structures
};

layout( std140, binding=6 ) buffer PosBoundary
{
vec4 Boundaries[ ]; // array of structures
};


layout( local_size_x = 128, local_size_y = 1, local_size_z = 1 ) in;


float toPower3(float x){ // GLSL pow doesn't handle negatives properly

    return sign(x) * pow(abs(x),3);

}

float toPower2(float x){ // GLSL pow doesn't handle negatives properly

    return pow(abs(x),2);

}

void main() {

uint gid = gl_GlobalInvocationID.x;

//if (gid%2 == 0){
//   PositionsPrevious[ gid ].xyz = PositionsPrevious[ gid ].xyz - vec3(0.0f,0.00001f,0.0f);
//
// if (PositionsPrevious[gid].y < -1.0f){
//
//  PositionsPrevious[gid].y = PositionsPrevious[gid].y + 2.0f;
//
//
// }


float dist;
float density = 0;

// Update Density
//Properties[gid].x = 0;
for(int i=0;i < 1024; i++){
// When the sign is known to be positive can just use pow
dist = distance(PositionsPrevious[gid].xy, PositionsPrevious[i].xy);

if (dist > h){
  density+=0.0;

}

  else if (dist > 0){

  density +=  toPower3(toPower2(h)-toPower2(dist))* (particleMass * 315/(64*3.14*pow(h,9)));

}




}

//  density = density* (0.01 * 315/(64*3.14*pow(1,9)));
    PropertiesPrevious[gid].x = density;//max(10,density);//density;

// Update Pressure
  PropertiesPrevious[gid].y = k*(pow(PropertiesPrevious[gid].x/restingDensity,7) - 1);



}
