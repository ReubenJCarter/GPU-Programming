/* 
Water ripples render, in glsl pixel shader language
written by Reuben Carter
*/
void main()
{
  vec4 color;
  vec2 c1 = vec2(Coord().x - 0.2, Coord().y - 0.2 );
  vec2 c2 = vec2(Coord().x - 0.8, Coord().y - 0.8);
  float sin1 = (1.0 / (length(c1) * 3.0) ) * sin(length(c1) * 2.0 * 3.14159 * 20.0)/2.0 +0.5;
  float sin2 = (1.0 / (length(c2) * 3.0) ) * sin(length(c2) * 2.0 * 3.14159 * 30.0)/2.0 +0.5;

  color = vec4(0.0, 0.5 * (sin1 + sin2) / 2.0, 0.8 * (sin1 + sin2) / 2.0, 1.0);
  
  gl_FragColor = color;
}