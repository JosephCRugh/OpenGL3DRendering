#version 130

in vec3 position;

uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;

void main()
{
  gl_Position = perspectiveMatrix * viewMatrix * vec4(position.xyz, 1);
}
