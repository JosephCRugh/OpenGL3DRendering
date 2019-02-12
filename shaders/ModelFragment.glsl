#version 130

in vec3 lightEffect;

out vec4 coloring;
uniform float ambient;

void main()
{
  coloring = vec4((lightEffect + ambient) * vec3(1.0f, 1.0f, 1.0f), 1.0f);
}
