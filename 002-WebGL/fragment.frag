#version 300 es

// fragment shaders don't have a default precision so we need
// to pick one.
precision mediump float;

// This is the color input from the vertex shader
in vec3 fragmentColor;

// We need to declare an output for the fragment shader
out vec4 outColor;

void main() {
    outColor = vec4(fragmentColor, 1.0);
}
