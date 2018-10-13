#version 130

in vec4 vposition;
in vec4 vcolor;
out vec4 color;
void main()
{
  color = vcolor;
  gl_Position = vposition;
}