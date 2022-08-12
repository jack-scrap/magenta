#version 400

in vec3 _pos;

void main() {
	gl_FragColor = vec4(vec3(_pos.y, 0.0, 1.0), 1.0);
}
