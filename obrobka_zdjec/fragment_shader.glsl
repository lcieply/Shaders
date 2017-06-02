#version 330 core 
uniform sampler2D myTextureSampler;
out vec4 pixelColors;

//argument pochodz¹cy z Vertex_Shadera
in vec3 pos;
in vec3 color;
uniform vec2 posCursor;

uniform float height;
uniform float width;
uniform float value;
void main()
{	
//bump mapping -> £ukasz
	vec2 coord = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
	vec2 trans = 0.005 *( gl_FragCoord.xy  - posCursor);
	
	float d = length(gl_FragCoord.xy - posCursor)/100.0f*((1-value)*2);
	trans *=d;

	vec2 coordLeft = vec2((gl_FragCoord.x-trans.x)/width, (gl_FragCoord.y-trans.y)/height);
	vec2 coordRight = vec2((gl_FragCoord.x+trans.x)/width, (gl_FragCoord.y+trans.y)/height);

	float texLeft = texture(myTextureSampler, coordLeft).g;
	float texRight = texture(myTextureSampler, coordRight).r;
	if(d!=0)
		pixelColors.rgba = 1.0/d* vec4(texture(myTextureSampler, coord).rgb, 1);
	else
		pixelColors.rgba = vec4(texture(myTextureSampler, coord).rgb, 1);

	pixelColors.rgba -= texLeft/2.0;
	pixelColors.rgba += texRight/2.0;
}