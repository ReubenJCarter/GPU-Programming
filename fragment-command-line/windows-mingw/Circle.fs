void main()
{
  vec4 color;
  vec2 temp = Coord() - vec2(0.5, 0.5);
  if(temp.x*temp.x + temp.y*temp.y < 0.25)
    color = vec4(0.0, 0.0, 0.0, 1.0);
  else 
    color = vec4(1.0, 1.0, 1.0, 1.0);
  gl_FragColor = color;
}