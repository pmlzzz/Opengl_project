#version 400            
uniform mat4 PVM;
uniform float time;
uniform int choice;
in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;  
in vec4 allcolor;
uniform  vec4 allcolor2;  
in vec4 move1;  
in vec4 move2; 
in vec4 move3; 
in vec4 move4; 
flat out vec4 color;
void main(void)
{	
	
	//vec4 col1=vec4(1.0,0.0,0.0,0.0);
	//vec4 col2=vec4(0.0,1.0,0.0,0.0);
	//vec4 col3=move4;
	mat4 translate=mat4(move1,move2,move3,move4);
	if(choice==1)
	{
		gl_Position = PVM*translate*vec4(pos_attrib, 1.0);
		color=allcolor;
	}
	else
	{
		gl_Position = PVM*vec4(pos_attrib, 1.0);
		color=vec4(allcolor2.x,allcolor2.y,allcolor2.z,0.0);
	}

	
}