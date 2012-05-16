void main()
{
  vec4 color;
  vec2 c1 = vec2(Coord().x, 0.0);
  vec2 c2 = vec2(Coord().x, Coord().y);
  vec2 c3 = vec2(-Coord().x, 1.0 - Coord().y);
  float sin1 = sin(length(c1) * 2.0 * 3.14159 * 10.0)/2.0 +0.5;
  float sin2 = sin(length(c2) * 2.0 * 3.14159 * 8.0)/2.0 +0.5;
  float sin3 = sin(length(c3) * 2.0 * 3.14159 * 4.0)/2.0 +0.5;
  color = vec4(sin1, sin2, sin3, 1.0);
  
  gl_FragColor = color;
}