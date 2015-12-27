
#version 430
#define h 0.2
#define particleMass 0.4
#define restingDensity 100.0
#define absoluteViscosity 200
#define timeStep 0.0025
#define gravity 0//-9.8
#define k 2500000
#define BALL_RADIUS 0.05
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

layout( std140, binding=7 ) buffer GridPos
{
vec4 Grid[ ]; // array of structures
};
//
 layout( std140, binding=8 ) buffer GridColor
 {
 vec4 GridCol[ ]; // array of structures
 };

 layout( std140, binding=9 ) buffer GridActivations
 {
 vec4 GridAct[ ]; // array of structures
 };


// layout( std140, binding=7 ) buffer Grd
// {
// vec4 Grid[ ]; // array of structures
// };



layout( local_size_x = 128, local_size_y = 1, local_size_z = 1 ) in;


float toPower3(float x){ // GLSL pow doesn't handle negatives properly

    return sign(x) * pow(abs(x),3);

}

float toPower2(float x){ // GLSL pow doesn't handle negatives properly

    return pow(abs(x),2);

}

float toPower4(float x){ // GLSL pow doesn't handle negatives properly

    return toPower2(toPower2(x));

}


void main() {

uint gid = gl_GlobalInvocationID.x;

float dist;

// Update Pressure Term
vec2 viscosityTerm = vec2(0,0);
vec2 pressureTerm = vec2(0,0);
float dDensitydt = 0.0;
vec2 rab;
vec2 gradKernal;
float Wab;
vec2 velocityCorrection;
float q = 0;

for(int i=0;i < 1024; i++){

  dist = distance(PositionsPrevious[gid].xy, PositionsPrevious[i].xy)/h;



  if ( (PropertiesPrevious[gid].x > 0 && (PropertiesPrevious[i].x > 0)) && (dist < h)&& (dist > 0)){

rab = PositionsPrevious[gid].xy-PositionsPrevious[i].xy;

  Wab = (315/64*3.14*pow(h,9)) * toPower3((pow(h,2) - pow(dist,2)));


    velocityCorrection = 1 * particleMass/(0.5*(Properties[gid].x + Properties[i].x)) * (VelocitiesPrevious[i].xy-VelocitiesPrevious[gid].xy) * Wab;
    pressureTerm += (-45/3.14*pow(h,6)) * (PropertiesPrevious[gid].y/pow(PropertiesPrevious[gid].x,2)+PropertiesPrevious[i].y/pow(PropertiesPrevious[i].x,2))*toPower2((h-dist)) * (PositionsPrevious[gid].xy-PositionsPrevious[i].xy)/dist;
    viscosityTerm += (h-dist)*((VelocitiesPrevious[i].xy - VelocitiesPrevious[gid].xy)/PropertiesPrevious[i].x);


    gradKernal =  1* 945/(32*3.14*pow(h,9)) * PositionsPrevious[gid].xy * toPower4(pow(h,2) - pow(dist,2)); // to power 3 works but to power 2 breaks ( but should be to power 2 according to literature)
    dDensitydt += particleMass * dot((VelocitiesPrevious[gid].xy-VelocitiesPrevious[i].xy), gradKernal);
  //  dDensitydt -= 0.00001*particleMass*dot(  ( (PropertiesPrevious[gid].x/PropertiesPrevious[i].x)-1 ) * (vec2(1/(toPower2(rab.x)+ 0.01*pow(h,2)),1/(toPower2(rab.y)+ 0.01*pow(h,2)) )  ),gradKernal);

  }

}

viscosityTerm *= (45/(3.14*pow(h,6)));
viscosityTerm *= (particleMass * absoluteViscosity);

if (Properties[gid].x > 0){
viscosityTerm *= 1.0f/PropertiesPrevious[gid].x;} //why is this brkoen??
else{
  viscosityTerm = vec2(0.0,0.0);
}

vec2 boundaryTerm = vec2(0.0f,0.0f);


float r0 = 0.08f;
float r = 0.0f;
float D = 0.00000050;
//for(int i=0; i < 1024; i++){

for(int j=0; j < 300; j++){


r = abs(distance(PositionsPrevious[gid].xy,Boundaries[j].xy));

if ((r > r0) || r == 0){
   boundaryTerm += vec2(0.0,0.0);
}
else{
boundaryTerm += D*(pow((r0/r),12) - pow((r0/r),6)) * (PositionsPrevious[gid].xy - Boundaries[j].xy ) / pow(r,2);
}
}

if(gid==0){

for(int j=0; j < 100; j++){
  Boundaries[j].z += 2*timeStep;//0.0011;
Boundaries[j].x = -0.7 + 0.2*(sin(Boundaries[j].z));
//Boundaries[j].x = -0.7 + 0.2*(2*(Boundaries[j].z/8 - floor(0.5 + Boundaries[j].z/8)));
}
}




//Velocities[gid].xy = VelocitiesPrevious[gid].xy + vec2(0,-9.8) + pressureTerm;// + viscosityTerm;
Velocities[gid].xy = 0.999999999*VelocitiesPrevious[gid].xy +   0.5*timeStep * vec2(0,-1.2) + timeStep*boundaryTerm - timeStep * pressureTerm + timeStep*viscosityTerm;//(viscosityTerm + pressureTerm);


if (dist < h){

}

Positions[gid].xy = PositionsPrevious[gid].xy + (Velocities[gid].xy + velocityCorrection)*timeStep;

Velocities[gid].xy = 0.999999999*VelocitiesPrevious[gid].xy +   0.5*timeStep * vec2(0,-1.9) + timeStep*boundaryTerm - timeStep * pressureTerm + timeStep*viscosityTerm;//(viscosityTerm + pressureTerm);

Properties[gid].x = PropertiesPrevious[gid].x  + timeStep * dDensitydt;
  Properties[gid].y =  k*(pow(PropertiesPrevious[gid].x/restingDensity,7) - 1);


float metaballActivation = 0.0f;

    for(int j = 0; j < 60; j++){

  for(int i = 0; i < 80; i++){


     GridAct[j*60+i].x = GridAct[j*60+i].x  + (pow(BALL_RADIUS,2) / toPower2(Grid[j*60+i].x - PositionsPrevious[gid].x) + toPower2(Grid[j*60+i].y - PositionsPrevious[gid].y));

  }

//GridColor[i].w = min(1.0f, metaballActivation);

  }




}
