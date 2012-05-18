uniform sampler2D image;
uniform int imageWidth;
uniform int imageHeight;

void main()
{
	vec4 color= TexturePixel(image);
	float factor = 2.0;
	color.x = float(int(color.x * factor)) / factor;
	color.y = float(int(color.y * factor)) / factor;
	color.z = float(int(color.z * factor)) / factor;
	
	gl_FragColor = color;
}