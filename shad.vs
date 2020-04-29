#version 400

in vec3 pos;

uniform mat4
	proj,
	view,
	model;

out vec3 colFrag;

void main() {
	gl_Position = proj * view * model * vec4(pos, 1.0);

	colFrag = pos;
}
