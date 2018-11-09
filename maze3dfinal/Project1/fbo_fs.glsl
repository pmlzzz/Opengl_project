#version 400

uniform sampler2D texture;
uniform int pass;
uniform int pick;

uniform bool no_edgedetection;

layout (location = 0)out vec4 fragcolor;          
in vec2 tex_coord;
in vec4 col;  
flat in int InstanceID;    
in vec3 vpos;
in vec3 tpos;
//function declarations

vec4 lighting(vec3 pos);
vec3 normal(vec3 pos);
uniform int xx;
uniform int zz;
uniform int obc;

void main(void)
{   
	if(pass == 1)
	{
		
		fragcolor = texture2D(texture, tex_coord)*col;
		fragcolor=fragcolor*lighting(vpos)*lighting(tpos);
		if(((InstanceID-InstanceID%20)/20%20)==0&&((InstanceID-InstanceID%400)/400)==zz&&(InstanceID%20)==xx&&obc==1)// y,z,x
		{fragcolor*=ivec4(1,0,0,1);fragcolor.a=0;}
		if(((InstanceID-InstanceID%20)/20%20)==0&&((InstanceID-InstanceID%400)/400)==zz+1&&(InstanceID%20)==xx&&obc==1)// y,z,x
		{fragcolor*=ivec4(1,0,0,1);}
		if(((InstanceID-InstanceID%20)/20%20)==0&&((InstanceID-InstanceID%400)/400)==zz-1&&(InstanceID%20)==xx&&obc==1)// y,z,x
		{fragcolor*=ivec4(1,0,0,1);}
		if(((InstanceID-InstanceID%20)/20%20)==0&&((InstanceID-InstanceID%400)/400)==zz&&(InstanceID%20)==xx+1&&obc==1)// y,z,x
		{fragcolor*=ivec4(1,0,0,1);}
		if(((InstanceID-InstanceID%20)/20%20)==0&&((InstanceID-InstanceID%400)/400)==zz&&(InstanceID%20)==xx-1&&obc==1)// y,z,x
		{fragcolor*=ivec4(1,0,0,1);}
		
	}
	else if(pass == 2)
	{

		fragcolor = texelFetch(texture, ivec2(gl_FragCoord), 0);
		
      
	}
	else
	{
		fragcolor = vec4(1.0, 0.0, 1.0, 1.0); //error
	}

}

vec4 lighting(vec3 pos)
{
	const vec3 light = vec3(1, 1, 1);
	const vec4 ambient_color = vec4(0.1, 0.2, 0.1, 1.0);
	const vec4 diffuse_color = vec4(1, 0.7, 0.5, 1.0);
	const vec4 spec_color = vec4(0.6, 0.6, 0.6, 1.0);
	vec3 n = normal(pos);
	vec3 v =normal(-pos);
	vec3 r = reflect(-light, n);

	vec3 h=normalize(light+v);
	float Eta=1.0;
	float c=dot(v, h);
	float g=sqrt(pow(Eta,2.0)+pow(c,2.0)-1);
	float F=pow((g-c),2.0)/(2.0*pow((g+c),2.0))*(1.0+pow(c*(g+c)-1,2.0)/pow(c*(g-c)+1,2.0));


	//vec4 I=ambient_color + diffuse_color*max(0.0, dot(n, light)) + spec_color*pow(max(0.0, dot(r, v)), 50.0);
	vec4 I=ambient_color + diffuse_color*max(0.0, dot(n, light)) + spec_color*pow(max(0.0, dot(r, v)), 50.0);
	return I;	
}





//normal vector of the shape we are drawing
vec3 normal(vec3 pos)
{

	return normalize(pos);
}












