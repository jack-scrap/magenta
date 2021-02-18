#version 400

in vec3 pos;

out vec3 _pos;

uniform mat4
	proj,
	view,
	model;

void main() {
	gl_Position = proj * view * model * vec4(pos, 1.0);

	_pos = pos;
}
