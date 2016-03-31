#version 330 core
layout( location = 0) in vec3 Position ;
layout( location =1) in vec3 Normal ;
layout( location =2) in vec2 TexCoord ;

uniform float time=0 ;
uniform mat4 T, R, M, P, MV, transMatrix;

// --- Add this to the declarations in the vertex shader
//layout( location = 1) in vec3 Color ;

out vec3 interpolatedColor ;
out vec3 interpolatedNormal ;
out vec2 st ;

void main () {

//gl_Position = T*R*vec4( Position*0.5, 1.0) ;
//gl_Position = T*vec4( Position, 1.0) ;

gl_Position = P*T*vec4( Position , 1.0) ; // Special , required output
//interpolatedNormal = Normal ; // Will be interpolated across the triangle

//vec3 transformedNormal=mat3(T)*Normal;
//interpolatedNormal=normalize(transformedNormal);

st=TexCoord; // Will also be interpolated across the triangle


//gl_Position = Mtemp * vec4(Position * sin(time), 1.0) ;
// And somewhere in its main() function , add this:

//interpolatedColor = Color; // Pass interpolated color to fragment shader

//sin(time);

}


