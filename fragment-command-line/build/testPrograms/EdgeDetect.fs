

vec4 BlackWhite(vec4 funcTexturePixel)
{
  	vec4 col= funcTexturePixel;
	float intensity;
	intensity=(col.x+col.y+col.z)/3.0;
	col.x=intensity;
	col.y=intensity;
	col.z=intensity;
	return col;
}

vec4 Binary(vec4 funcTexturePixel, float funcMinVal)
{
	vec4 col= funcTexturePixel;
	col.x=(col.x+col.y+col.z)/3.0;
	col.y=0.0;
	col.z=0.0;
	if (col.x>funcMinVal)
	{
		col.x=1.0;
		col.y=1.0;
		col.z=1.0;
	}
	else
	{
		col.x=0.0;
	}
	return col;
}

vec4 Threshold(vec4 funcTexturePixel, int funcFactor)
{
	vec4 col= funcTexturePixel;
	float val=col.x;
	col.x= float(int(val*float(funcFactor)))/float(funcFactor);
	val=col.y;
	col.y= float(int(val*float(funcFactor)))/float(funcFactor);
	val=col.z;
	col.z= float(int(val*float(funcFactor)))/float(funcFactor);
	return col*2.0;
}

vec4 Gradient(sampler2D funcTexture, int funcTextureUnit, int funcWidth, int funcHeight, int funcThresholdFactor, float funcGradientCutoff)
{
	vec4 n= TexturePixel(funcTexture, funcTextureUnit, Coord(CoordIntX(funcWidth), CoordIntY(funcHeight)-1, funcWidth, funcHeight));
	vec4 s= TexturePixel(funcTexture, funcTextureUnit, Coord(CoordIntX(funcWidth), CoordIntY(funcHeight)+1, funcWidth, funcHeight));
	vec4 e= TexturePixel(funcTexture, funcTextureUnit, Coord(CoordIntX(funcWidth)+1, CoordIntY(funcHeight), funcWidth, funcHeight));
	vec4 w= TexturePixel(funcTexture, funcTextureUnit, Coord(CoordIntX(funcWidth)-1, CoordIntY(funcHeight), funcWidth, funcHeight)); 
	vec4 me= TexturePixel(funcTexture, funcTextureUnit, Coord());
	n=BlackWhite(n);
	s=BlackWhite(s);
	e=BlackWhite(e);
	w=BlackWhite(w);
	me=BlackWhite(me);
	n=Threshold(n, funcThresholdFactor);
	s=Threshold(s, funcThresholdFactor);
	e=Threshold(e, funcThresholdFactor);
	w=Threshold(w, funcThresholdFactor);
	me=Threshold(me, funcThresholdFactor);
	n=Binary(n, 0.01);
	s=Binary(s, 0.01);
	e=Binary(e, 0.01);
	w=Binary(w, 0.01);
	me=Binary(me, 0.1);
	float dn= abs(n.x-me.x);
	float ds= abs(s.x-me.x);
	float de= abs(e.x-me.x);
	float dw= abs(w.x-me.x);
	float tempIntesity=(dn+dw+ds+de)/4.0;
	if(tempIntesity>funcGradientCutoff)
		tempIntesity=1.0;
	else
		tempIntesity=0.0;
	vec4 result= vec4(tempIntesity, tempIntesity, tempIntesity, 1.0);
	return result;
}

uniform sampler2D image;
uniform int imageWidth;
uniform int imageHeight;

void main()
{
	vec4 color;
	color= Gradient(image, 0, imageWidth, imageHeight, 10, 0.001);
	gl_FragColor = color;
}