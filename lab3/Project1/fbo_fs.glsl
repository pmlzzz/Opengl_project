#version 400

uniform sampler2D texture;
uniform sampler2D texture2;
uniform int pass;
uniform int pick;

uniform bool no_LOD;

layout (location = 0)out vec4 fragcolor; 
layout (location = 1)out vec4 fragcolor2;          
in vec2 tex_coord;
in vec4 col;  
flat in int InstanceID;      
void main(void)
{   
	if(pass == 1)
	{

		
		if(no_LOD)
		{
			fragcolor = texture2D(texture, tex_coord)*col;
		}
		else
		{
			
			if(textureQueryLod(texture, tex_coord).x>=3)
			{
				fragcolor =vec4(1.0,0.0,0.0,1.0);
			}
			else if(textureQueryLod(texture, tex_coord).x==0)
			{
				fragcolor =vec4(0.0,1.0,0.0,1.0);
			}
			else
			{
				fragcolor = textureLod(texture, tex_coord, textureQueryLod(texture, tex_coord).x)*col;
			}
		}
		
	}
	else if(pass == 2)
	{
      
		fragcolor = texelFetch(texture, ivec2(gl_FragCoord), 0);

	}

}




















