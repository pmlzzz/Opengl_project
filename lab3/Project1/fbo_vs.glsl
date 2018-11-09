#version 400            
uniform mat4 PVM;
uniform int pass;
uniform int pick;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;
uniform  vec4 color;
out vec2 tex_coord;  
out vec4 col;
flat out int InstanceID;
void main(void)
{
	if(pass == 1)
	{
		gl_Position = PVM*vec4(pos_attrib, 1.0);
		gl_Position.xyz*=0.5;
		if(pick==gl_InstanceID*10+10){gl_Position.xyz*=2;}
		gl_Position.x+=1.5;
		gl_Position.y+=1.5;
		gl_Position.x-=1.5*(gl_InstanceID%3);
		gl_Position.y-=1.5*(gl_InstanceID-gl_InstanceID%3)/3;
		tex_coord = tex_coord_attrib;
		InstanceID = gl_InstanceID;
		
	}
	else
	{
		gl_Position = vec4(pos_attrib, 1.0);
      tex_coord = 0.5*pos_attrib.xy + vec2(0.5);
	}
	col=color;
}