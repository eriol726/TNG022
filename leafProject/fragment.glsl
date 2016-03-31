#version 330 core

//uniform float time ;
uniform mat4 T;
// --- Add this to the declarations in the fragment shader
in vec3 interpolatedColor ;
in vec3 interpolatedNormal ;
uniform sampler2D tex ; // A uniform variable to identify the texture
in vec2 st ; // Interpolated texture coords , sent from vertex shader

out vec4 finalcolor ;

void main () {

//finalcolor = vec4(0.6 , 0.1 , 0.5 , 1.0) ;
//finalcolor = vec4(0.6 , 0.1 , sin(time) , 1.0) ;

vec3 lightDirection = vec3(1.0 , 1.0 , 1.0) ;
float shading = dot( interpolatedNormal , lightDirection ) ;
shading = max(0.0 , shading ); // Clamp negative values to 0.0
finalcolor = vec4(vec3(shading) , 1.0) ;

vec3 L = normalize(mat3(T)*vec3(1, 1, 1));
vec3 V = normalize(vec3(0, 0, 1));
vec3 N = interpolatedNormal;
float n = 100;
vec3 ka = vec3(1.0, 0.4, 0.5);
vec3 kd = vec3(0.0, 0.0, 1.0);
vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 Ia = vec3(0.2, 0.2, 0.2);
vec3 Id = vec3(1.0, 1.0, 1.0);
vec3 Is = vec3(1.0, 1.0, 1.0);

// vec3 L is the light direction
// vec3 V is the view direction - (0 ,0 ,1) in view space
// vec3 N is the normal
// vec3 R is the computed reflection direction
// float n is the "shininess" parameter
// vec3 ka is the ambient reflection color
// vec3 Ia is the ambient illumination color
// vec3 kd is the diffuse surface reflection color
// vec3 Id is the diffuse illumination color
// vec3 ks is the specular surface reflection color
// vec3 Is is the specular illumination color
// This assumes that N, L and V are normalized.

vec3 R = 2.0*dot(N ,L) *N - L; // Could also have used the function reflect()
float dotNL = max(dot(N , L) , 0.0) ; // If negative , set to zero
float dotRV = max(dot(R , V) , 0.0) ;
vec4 texturecolor = texture( tex , st ) ;
vec3 shadedcolor = Ia * vec3(texturecolor) + Id * vec3(texturecolor) * dotNL + Is * ks *pow( dotRV , n);

//vec3 shadedcolor = Ia * ka + Id * kd * dotNL + Is * ks *pow( dotRV , n);
finalcolor = vec4(shadedcolor , 1.0) ;
//finalcolor = texture( tex , st ) ;
//finalcolor = vec4(1.0, 1.0, 1.0, 1.0);

//finalcolor = texture( tex , st ) ; // Use the texture to set the surface color


// And in its main() function , set the output color like this:
//finalcolor = vec4( interpolatedColor , 1.0) ;

//finalcolor = vec4(vec3(interpolatedNormal.z) , 1.0) ;

//finalcolor = vec4( interpolatedColor * cos(7*time) , 1.0) ;
}
