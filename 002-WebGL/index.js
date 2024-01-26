/*
 * WebGL is a GPU Rasterization Engine: it draws points, lines and triangles
 * thanks to the graphics card.
 *
 * Therefore, you need to provide code (written in GLSL) that runs on the GPU,
 * these programs are called shaders. Most of WebGL code (CPU) is to setup those
 * programs.
 *
 *    - Vertex shader: runs for every vertex/point to calculate their final
 *      position on the screen.
 *    - Fragment shader: runs for every pixel that has to be rasterized
 *      generating a color. It depends on the output of the vertex shader.
 * 
 * Among others.
 *
 * There are 4 forms to send data to the shaders:
 *
 *    - Attributes, buffers, and vertex arrays: binary data uploaded to the GPU
 *      (positions, normals, texture coordinates, vertex colors, ...).
 * 
 *      Attributes specify how to pull data out of the buffer: type, offset and size.
 *      Each time a shader is executed, the next value of the buffer is pulled.
 *      You can think them as the input to the shader.
 *      
 *    - Uniforms: constant global variables to the shader
 *    - Textures: arrays of data with random access
 *    - Varyings: a way to pass data from the vertex shader to the fragment shader.
 * 
 * WebGL follows these steps (or pipeline):
 * 
 *    - Vertex shader execution
 *    - Primitive assembly: the GPU figures out which vertices go with which triangles.
 *    - Rasterization: the GPU calculate all the pixels that need to be updated.
 *    - Fragment shader execution
 *    - Output merging: finally, the GPU blends the previous image with the new one.
 * 
 * Vertex array objects (VAOs): just an snapshot of an input assembler state.
 * This can include diferent data for each object, so switching between different
 * objects is easy.
 *
 * References:
 *    - WebGL2 tutorial: https://webgl2fundamentals.org/
 *    - Youtube: Indigo Code WebGL2 Series
 *        * https://youtu.be/y2UsQB3WSvo
 *        * https://youtu.be/lLa6XkVLj0w
 * 
 * Useful links:
 *    - MDN Reference: https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API
 */

// JavaScript does not use types, but WebGL do require them
const trianglePositions = new Float32Array([
    // Equilateral triangle with center in (0, 0) size of 10px
    // Thanks to the shapeSize uniform, these values are in pixels
    0, 4.184316, // l/2 (sqrt(2) - sqrt(3)/3)
    5, -2.886751, // sqrt(3)/6 l
    -5, -2.886751
]);

const rbgTriangleColors = new Uint8Array([
    255, 0, 0, // Red
    0, 255, 0, // Green
    0, 0, 255  // Blue
]);

async function readSrc(path) {
    const response = await fetch(path);
    return await response.text();
}

function showError(msg) {
    const box = document.getElementById('error-box');
    const error = document.createElement('p');
    error.innerText = msg;
    error.classList.add('error-log');
    box.appendChild(error);
    console.error(msg);
}

function createShader(gl, type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    const success = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
    if (success) {
        return shader;
    }

    showError('[Compiling shader]: ' + gl.getShaderInfoLog(shader));
    gl.deleteShader(shader);
}

function createProgram(gl, vertexShader, fragmentShader) {
    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);

    const success = gl.getProgramParameter(program, gl.LINK_STATUS);
    if (success) {
        return program;
    }

    showError('ERROR linking program: ' + gl.getProgramInfoLog(program));
    gl.deleteProgram(program);
}

async function main() {
    /*
     * First, we must get the canvas element of the page and get
     * a surface to render to.
     */
    /** @type {HTMLCanvasElement|null} */
    const canvas = document.getElementById('canvas');
    const gl = canvas.getContext('webgl2');
    if (!gl) {
        showError('ERROR: WebGL2 is not avaliable');
        return;
    }

    // Set canvas size
    // Output merger
    canvas.width = 800;
    canvas.height = 600;
    canvas.clientWidth = canvas.width;
    canvas.clientHeight = canvas.height;
    gl.canvas.width = canvas.width;
    gl.canvas.height = canvas.height;
    // Rasterizer: specifies the region we want to draw
    gl.viewport(0, 0, canvas.width, canvas.height);

    // Clear the screen
    // This means to fill the color buffer and depth buffer with a color
    // TODO: does not work
    gl.clearColor(0.08, 0.08, 0.08, 1.0); // RGBA
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    /*
     * Then, we will create the shaders (reading the source code and
     * compiling them) and setup the required data for them (coordinate
     * positions of the triangle).
     */

    //// Shaders setup ////
    const vertexSrc   = await readSrc('vertex.vert');
    const fragmentSrc = await readSrc('fragment.frag');

    const vertexShader   = createShader(gl, gl.VERTEX_SHADER, vertexSrc);
    const fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentSrc);
    const program = createProgram(gl, vertexShader, fragmentShader);
    gl.useProgram(program);

    //// Buffers setup ////
    // This buffer will hold the coordinates of a triangle we are going to render
    const positionBuffer = gl.createBuffer();         // This is a memory chunk of the GPU
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);   // "Use" the created buffer
    gl.bufferData(gl.ARRAY_BUFFER, trianglePositions, gl.STATIC_DRAW); // Hint for optimizations: we will not change this data

    // This other buffer will hold the colors
    // const colorBuffer = gl.createBuffer();
    // gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    // gl.bufferData(gl.ARRAY_BUFFER, colorBuffer, gl.STATIC_DRAW);

    // Tell how to read the previous buffer
    // First, tell where in the vertex shader we reference this buffer
    const positionAttribLocation = gl.getAttribLocation(program, 'inputPosition');
    // const colorAttribLocation = gl.getAttribLocation(program, 'inputColor');
    if (positionAttribLocation < 0 || colorAttribLocation < 0) {
        showError('Failed to get attrib location for the position or color');
        return;
    }

    // Activate the attributes
    gl.enableVertexAttribArray(positionAttribLocation); 

    // Input assembler
    // Describe the buffer contents
    const size = 2;          // 2 components per iteration
    const type = gl.FLOAT;   // the data is 32bit floats
    const normalize = false; // don't normalize the data
    const stride = 0;        // 0 = move forward size * sizeof(type) each iteration to get the next position
    const offset = 0;        // start at the beginning of the buffer
    gl.vertexAttribPointer(positionAttribLocation, size, type, normalize, stride, offset);

    // gl.enableVertexAttribArray(colorAttribLocation);
    // gl.vertexAttribPointer(colorAttribLocation, 3, gl.INT, false, 0, 0);

    //// Uniforms setup ////
    const shapeLocationUniform = gl.getUniformLocation(program, 'shapeLocation');
    const shapeSizeUniform = gl.getUniformLocation(program, 'shapeSize');
    const canvasSizeUniform = gl.getUniformLocation(program, 'canvasSize');

    if (!shapeLocationUniform || !shapeSizeUniform || !canvasSizeUniform) {
        showError('Failed to get uniform locations');
        return;
    }

    // Define data for the uniforms
    gl.uniform2f(canvasSizeUniform, canvas.width, canvas.height); // vec2 of floats
    gl.uniform2f(canvasSizeUniform, canvas.width, canvas.height);

    //// Draw call ////
    // When implementing a game loop, there are the only functions that should go there
    gl.uniform1f(shapeSizeUniform, 20);
    gl.uniform2f(shapeLocationUniform, 300, 400);
    gl.drawArrays(gl.TRIANGLES, 0, 3);
}

try {
    main();
} catch(e) {
    showError('JavaScript Exception: ' + e);
}
