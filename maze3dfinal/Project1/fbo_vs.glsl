#version 400            
uniform mat4 PVM;
uniform int pass;
uniform int pick;

in vec3 pos_attrib;
in vec3 tex_coord_attrib;
in vec3 normal_attrib;
uniform  vec4 color;
out vec2 tex_coord;  
out vec4 col;
flat out int InstanceID;
out vec3 vpos;
out vec3 tpos;
out vec4 oPosCam;
out vec3 oNormalCam;
out float outValue;

uniform int xx;
uniform int zz;
void main(void)
{	
	
	outValue=zz*400+xx;
	if(pass == 1)
	{
		gl_Position = vec4(pos_attrib, 1.0);
		gl_Position.xyz*=0.5;
		if(pick==gl_InstanceID*10+10){gl_Position.xyz*=2;}
		gl_Position.x+=1*10;
		gl_Position.y+=1*10;
		gl_Position.z+=1*10;
		gl_Position.x-=1*(gl_InstanceID%20);
		gl_Position.y-=1*((gl_InstanceID-gl_InstanceID%20)/20%20);
		

		gl_Position.z-=1*((gl_InstanceID-gl_InstanceID%400)/400);
		if(((gl_InstanceID-gl_InstanceID%20)/20%20)==0&&((gl_InstanceID-gl_InstanceID%400)/400)==zz&&(gl_InstanceID%20)==xx)// y,z,x
		{
			gl_Position.y-=0;
			
		
		}
		gl_Position = PVM*gl_Position;
		tex_coord = tex_coord_attrib.xy;
		InstanceID = gl_InstanceID;
		
	}
	else
	{
		gl_Position = vec4(pos_attrib, 1.0);
      tex_coord = 0.5*pos_attrib.xy + vec2(0.5);
	}
	col=color;
	vpos = gl_Position.xyz;
	tpos = (vec4(tex_coord_attrib, 1.0)).xyz;
}

