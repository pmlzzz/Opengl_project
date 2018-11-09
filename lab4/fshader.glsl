#version 440

uniform float time;
uniform mat4 Q;
uniform sampler2D backfaces;
uniform sampler2D frontfaces;
uniform int pass;
uniform int option;
uniform int gamma;
in vec3 vpos;

out vec4 fragcolor;   

//function declarations
vec4 raytracedcolor(vec3 rayStart, vec3 rayStop);
vec4 lighting(vec3 pos, vec3 rayDir);
float distToShape(vec3 pos);
vec3 normal(vec3 pos);

void main(void)
{   
	//backfaces get position rendered to texture
	if(pass == 1 || pass == 2)
	{
		fragcolor = vec4(vpos, 1.0);
		return;	
	}

	//uncomment to show backface texture
	//fragcolor = texelFetch(backfaces, ivec2(gl_FragCoord.xy), 0);
	//return;

	vec3 rayStart = vpos.xyz;

	vec3 rayStop = texelFetch(backfaces, ivec2(gl_FragCoord.xy), 0).xyz;
	fragcolor = raytracedcolor(rayStart, rayStop);

   if(fragcolor.a==0.0) discard;
   
   fragcolor = pow(fragcolor, vec4(0.45, 0.45, 0.45, 1.0));
   if(gamma==1)
   {fragcolor.rgb = pow(fragcolor.rgb, vec3(1.0/2.0,1.0/2.0,1.0/2.0));}
}

// trace rays until they intersect the surface
vec4 raytracedcolor(vec3 rayStart, vec3 rayStop)
{
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	float scale = 1.0/Q[0].x;
	int MaxSamples = 1000;

	vec3 rayDir = normalize(rayStop-rayStart);
	float travel = distance(rayStop, rayStart);
	float stepSize = travel/MaxSamples;
	vec3 pos = rayStart;
	vec3 step = rayDir*stepSize;
	
	for (int i=0; i < MaxSamples && travel > 0.0; ++i, pos += step, travel -= stepSize)
	{
		float dist = distToShape(pos);

		stepSize = dist;
		step = rayDir*stepSize;
		
		if(dist<=0.001/scale)
		{
			color = lighting(pos, rayDir);
			return color;
		}	
	}
	return color;
}

//compute lighting on the intersected surface
vec4 lighting(vec3 pos, vec3 rayDir)
{
	const vec3 light = vec3(1.0/1.7, 1.0/1.7, 1.0/1.7);
	const vec4 ambient_color = vec4(0.1, 0.2, 0.1, 1.0);
	const vec4 diffuse_color = vec4(0.5, 0.7, 0.5, 1.0);
	const vec4 spec_color = vec4(0.6, 0.6, 0.6, 1.0);
	vec3 n = normal(pos);
	vec3 v = -rayDir;
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


//shape function declarations
float sdSphere( vec3 p, float s );
float sdTorus( vec3 p, vec2 t );
float udBox( vec3 p, vec3 b );
float opTwist( vec3 p );
float opU( float d1, float d2 );
float opS( float d1, float d2 );
float opI( float d1, float d2 );
// For more distance functions see
// http://iquilezles.org/www/articles/distfunctions/distfunctions.htm

// Soft shadows
// http://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm

// WebGL example and a simple ambient occlusion approximation
// https://www.shadertoy.com/view/Xds3zN


//distance to the shape we are drawing
float distToShape(vec3 pos)
{
	float radius = 0.4;
	if(option==0)
	{
		return sdSphere(pos, radius);
	}
	else if (option==1)
	{
		return sdTorus(pos, vec2(0.20,0.05));
	}
	else if (option==2)
	{
		return opU(opS(sdSphere(pos, 0.2),udBox(pos, vec3(0.1,0.1,0.25))),opTwist(  pos ));
	}

}

//normal vector of the shape we are drawing
vec3 normal(vec3 pos)
{
	const float h = 0.001;
	const vec3 Xh = vec3(h, 0.0, 0.0);	
	const vec3 Yh = vec3(0.0, h, 0.0);	
	const vec3 Zh = vec3(0.0, 0.0, h);	

	return normalize(vec3(distToShape(pos+Xh)-distToShape(pos-Xh), distToShape(pos+Yh)-distToShape(pos-Yh), distToShape(pos+Zh)-distToShape(pos-Zh)));
}
  
  
// shape function definitions
                
float sdSphere( vec3 p, float s )
{
	return length(p)-s;
}
float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}
float udBox( vec3 p, vec3 b )
{
  return length(max(abs(p)-b,0.0));
}
float opTwist( vec3 p )
{
    float c = cos(20.0*p.y);
    float s = sin(20.0*p.y);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*p.xz,p.y);
    return sdTorus(q, vec2(0.20,0.05));
}

float opU( float d1, float d2 )
{
    return min(d1,d2);
}

float opS( float d1, float d2 )
{
    return max(-d1,d2);
}

float opI( float d1, float d2 )
{
    return max(d1,d2);
}





