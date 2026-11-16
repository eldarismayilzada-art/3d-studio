#version 450 core

// vPosition is a vertex attribute. Each time the vertex shader runs;
// once per vertex, vPosition will hold the position of that vertex,
// fetched from the VBO according to the VAO recipe for that attribute.

in vec3 vPosition;
uniform mat4 M;

// The shader variable gl_Position is a built-in, implicit output
// value. Is must receive the position in clip coordinates. This
// example contains no user-defined output variables

void
main()
{
    gl_Position =  M*vec4(vPosition, 1);
}
