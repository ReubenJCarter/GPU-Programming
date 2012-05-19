void main()
{
  vec4 color;
  vec2 c1 = vec2(Coord().x, Coord().y);
  vec2 c2 = vec2(Coord().x, -Coord().y + 1.0);
  float sin1 = sin(length(c1) * 2.0 * 3.14159 * 80.0)/2.0 +0.5;
  float sin2 = sin(length(c2) * 2.0 * 3.14159 * 80.0)/2.0 +0.5;
  color = vec4((sin1 + sin2)/2.0, 0.0, 0.0, 1.0);
  
  gl_FragColor = color;
}