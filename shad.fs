#version 400

in vec3 colFrag;
out vec4 col;

void main() {
	col = vec4(colFrag, 1.0);
}
