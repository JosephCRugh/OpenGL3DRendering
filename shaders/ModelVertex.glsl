#version 130

in vec3 position;
in vec3 normal;

struct Light {
  vec3 position;
  vec3 color;
};

#define NUM_LIGHTS #Inserted at runtime.
uniform Light lights[NUM_LIGHTS];

uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 lightEffect;

void main()
{
  vec4 worldCoords = modelMatrix * vec4(position.xyz, 1);
  gl_Position = perspectiveMatrix * viewMatrix * worldCoords;

  vec3 v3WorldCoords = vec3(worldCoords);
  lightEffect = vec3(0);
  for (int i = 0; i < NUM_LIGHTS; i++)
  {
    vec3 lightDirection = normalize(lights[i].position - v3WorldCoords);
    lightEffect += max(dot(normal, lightDirection), 0.0) * lights[i].color;
  }
}
