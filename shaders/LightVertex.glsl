#version 130

in vec3 position;

uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
  gl_Position = perspectiveMatrix * viewMatrix * modelMatrix * vec4(position.xyz, 1);
}
