#version 300 es

// Middle precision on float variables
precision mediump float;
 
// An attribute is an input (in) to a vertex shader.
// It will receive data from a buffer as specified in
// the WebGL code.
in vec2 inputPosition;
in vec3 inputColor;

// Uniforms inputs
// The default values are 0
uniform vec2 canvasSize;
uniform vec2 shapeLocation;
uniform float shapeSize;

// Output to the fragment shader.
// Both variables must have the same name.
out vec3 fragmentColor;
 
// All shaders have a main function
void main() {
    // For the fragment color, just copy the value
    // The GPU will interpolate the values for every fragment
    fragmentColor = inputColor;

    // Calculate the transformed position
    vec2 position = inputPosition * shapeSize + shapeLocation;

    // Calculate the clip space [-1, 1] position
    //                [0, 1]              [0, 2] [-1, 1]
    vec2 clipSpace = (position / canvasSize) * 2.0 - 1.0;

    // Clip space positions
    //    - ( 0,  0) is the center of the screen
    //    - (-1, -1) is the bottom left corner
    //    - ( 1,  1) is the top right corner

    // gl_Position is a special variable a vertex shader
    // is responsible for setting. It is a vec4:
    //   - The first and second components are the x and
    //     y coordinates, in the range [-1, 1].
    //   - Depth information (Z) [-1, 1]: determines which
    //     pixel goes on top of another.
    //   - W: the rest of the components will be devided
    //     by this value
    gl_Position = vec4(clipSpace, 0.0, 1.0);
}