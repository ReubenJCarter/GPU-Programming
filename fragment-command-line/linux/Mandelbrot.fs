/* 
Mandelbrot set render, in glsl pixel shader language
written by Reuben Carter
*/

void main()
{
  float scale = 0.6;
  vec4 colour = vec4(0.0, 0.0, 0.3, 1.0);
  vec2 c = Coord()/scale;
  c.x -=1.5;
  c.y -=1.0;
  vec2 z = c;
  float x_temp;
  
  for(int i = 0; i < 100; i++)
  {
    x_temp = z.x;
    z.x = z.x * z.x - z.y * z.y + c.x;
	z.y = x_temp * z.y * 2.0 + c.y;
    if((z.x*z.x + z.y*z.y) > 4.0)
	{
      	colour.x =  1.0/(float(i)/8.0);
		colour.y =  1.0/(float(i)/8.0);
		break;
    }
  }
  gl_FragColor = colour;
}