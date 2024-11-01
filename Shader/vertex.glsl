#version 330 core
layout(location = 0) in vec3 aPos; // Vertex position

uniform mat4 transform; // Transformation matrix

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
}
