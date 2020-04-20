#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "disp.h"
#include "prog.h"
#include "util.h"

int main() {
	Disp disp("asdf", 800, 600);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vtc[] = {
		-1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, 1.0f, 
		-1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, 1.0f, -1.0f, 
		1.0f, -1.0f, 1.0f, 
		-1.0f, -1.0f, -1.0f, 
		1.0f, -1.0f, -1.0f, 
		1.0f, 1.0f, -1.0f, 
		1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, 1.0f, 1.0f, 
		-1.0f, 1.0f, -1.0f, 
		1.0f, -1.0f, 1.0f, 
		-1.0f, -1.0f, 1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, 1.0f, 1.0f, 
		-1.0f, -1.0f, 1.0f, 
		1.0f, -1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f, -1.0f, -1.0f, 
		1.0f, 1.0f, -1.0f, 
		1.0f, -1.0f, -1.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f, -1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, -1.0f, 
		-1.0f, 1.0f, -1.0f, 
		1.0f, 1.0f, 1.0f, 
		-1.0f, 1.0f, -1.0f, 
		-1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		-1.0f, 1.0f, 1.0f, 
		1.0f, -1.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof vtc, vtc, GL_STATIC_DRAW);

	// matrix
	glm::mat4
		proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f),
		view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
		model = glm::mat4(1.0f);

	// shader
	Prog prog("shad", "shad");

	/// attribute
	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	/// uniform
	GLint uniProj = glGetUniformLocation(prog._id, "proj");
	glEnableVertexAttribArray(uniProj);

	GLint uniView = glGetUniformLocation(prog._id, "view");
	glEnableVertexAttribArray(uniView);

	GLint uniModel = glGetUniformLocation(prog._id, "model");
	glEnableVertexAttribArray(uniModel);

	// initialize
	prog.use();

	while (true) {
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		disp.clear(0, 0, 0, 1);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		disp.update();
	}
}
