#version 400

uniform sampler2D texture;
uniform sampler2D texture2;
uniform int pass;
uniform int pick;

uniform bool no_edgedetection;

layout (location = 0)out vec4 fragcolor; 
layout (location = 1)out vec4 fragcolor2;          
in vec2 tex_coord;
in vec4 col;  
flat in int InstanceID;      
void main(void)
{   
	if(pass == 1)
	{
		
		fragcolor = texture2D(texture2, tex_coord)*col;
		fragcolor2 =vec4((InstanceID+1)*10/255.0,0.0,0.0,1);
		//fragcolor=(left-right)*(left-right)+(above-below)*(above-below);
		if(pick==InstanceID*10+10){fragcolor=vec4(0.0,1.0,0.0,1.0);}
	}
	else if(pass == 2)
	{
      if(no_edgedetection)
      {
		
		fragcolor = texelFetch(texture, ivec2(gl_FragCoord), 0);
		
      }
	  else{
		fragcolor = vec4(0.0);
	    float d=1.0f;
		vec4 left = texelFetch(texture, ivec2(gl_FragCoord.x-d,gl_FragCoord.y), 0);
		vec4 right = texelFetch(texture, ivec2(gl_FragCoord.x+d,gl_FragCoord.y), 0);
		vec4 above = texelFetch(texture, ivec2(gl_FragCoord.x,gl_FragCoord.y-d), 0);
		vec4 below = texelFetch(texture, ivec2(gl_FragCoord.x,gl_FragCoord.y+d), 0);
		fragcolor=(left-right)*(left-right)+(above-below)*(above-below);
	  }
      
	}
	else
	{
		fragcolor = vec4(1.0, 0.0, 1.0, 1.0); //error
	}

}




















