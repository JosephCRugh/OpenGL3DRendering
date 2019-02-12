#version 130

in vec3 position;
in vec3 normal;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 lightEffect;

void main()
{
  vec4 worldCoords = modelMatrix * vec4(position.xyz, 1);
  gl_Position = perspectiveMatrix * viewMatrix * worldCoords;

  vec3 lightDirection = normalize(lightPosition - vec3(worldCoords));

  lightEffect = max(dot(normal, lightDirection), 0.0) * lightColor;
}
