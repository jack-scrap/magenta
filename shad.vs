#version 400

in vec3 pos;

out vec3 _pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform unsigned int t;

void main() {
	gl_Position = proj * view * model * vec4(pos + vec3(0.0, sin(float(t) / 50), 0.0), 1.0);

	_pos = pos;
}
