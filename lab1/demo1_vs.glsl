#version 400            
uniform mat4 PVM;
uniform float time;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;  
flat out int InstanceID;
out vec3 n;
out vec3 pos;
void main(void)
{	
	gl_Position = PVM*vec4(pos_attrib, 1.0);
	gl_Position.xyz*=0.5;
	gl_Position.x+=2+0.05*sin(15*gl_Position.y+time);
	gl_Position.y+=2-0.05*sin(15*gl_Position.x+time);
	gl_Position.x-=2*(gl_InstanceID%3);
	gl_Position.y-=2*(gl_InstanceID-gl_InstanceID%3)/3;
	n=normal_attrib;
	pos=pos_attrib;
	InstanceID = gl_InstanceID;
}