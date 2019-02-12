#version 130

out vec4 coloring;

uniform vec3 lightColor;

void main()
{
  coloring = vec4(lightColor.xyz, 1);;
}
