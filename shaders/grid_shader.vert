// Vertex shader
#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1 ) in vec3 in_color;

out vec3 color;
uniform mat4 mvp;
uniform vec3 direction;

void main()
{
    vec3 p = pos + ( gl_InstanceID - 50  ) * 0.2 * direction;
    gl_Position = mvp * vec4(p.x,p.y,p.z,1.0); 
    color = in_color;
}
