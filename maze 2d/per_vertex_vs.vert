#version 400            
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float time;

// per-vertex lighting in world space
uniform vec3 light_pos = vec3(0.0, 3.0, 0.0); //world space light pos

uniform vec4 ka = vec4(0.13, 0.13, 0.13, 1.0); // material ambient color
uniform vec4 La = vec4(1.0, 1.0, 1.0, 1.0); // light source ambient color

uniform vec4 kd = vec4(0, 0.7, 0.7, 1.0); // material diffuse color
uniform vec4 Ld = vec4(1.0, 1.0, 1.0, 1.0); // light source diffuse color

uniform vec4 ks = vec4(0.5, 0.5, 0.5, 1.0); // material specular color
uniform vec4 Ls = vec4(1.0, 1.0, 1.0, 1.0); // light source specular color

uniform float shininess = 10.0;

in vec3 pos_attrib;        //mesh vertices in object space
in vec3 normal_attrib;     //mesh normals in object space
in vec2 tex_coord_attrib;  //texture_coordinates
uniform  vec4 color;
out vec4 I;

void main(void)
{
   vec4 p = M*vec4(pos_attrib, 1.0);   //world space vertex
	gl_Position = P*V*p;                //clip space vertex position
   
   vec4 cam_pos = inverse(V)*vec4(0.0, 0.0, 0.0, 1.0);   //world space camera/eye position

   vec3 n = normalize(M*vec4(normal_attrib, 0.0)).xyz;   //world space normal 
   vec3 l = normalize(light_pos - p.xyz);                //world space light vector
   vec3 v = normalize(cam_pos.xyz - p.xyz);              //world space view vector
   vec3 r = reflect(-l, n);                              //world space reflection vector

   I = ka*La + color*Ld*max(dot(n, l), 0.0) + ks*Ls*max(pow(dot(r, v), shininess), 0.0);
}