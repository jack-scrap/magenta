#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "disp.h"
#include "prog.h"
#include "util.h"

std::vector<std::string> split(std::string buff, char delim) {
	std::vector<std::string> tok;

	std::stringstream s(buff);
	std::string seg;
	while (std::getline(s, seg, delim)) {
		tok.push_back(seg);
	}

	return tok;
}

std::vector<std::string> rd(std::string fName) {
	std::ifstream in;
	in.open(fName);

	std::vector<std::string> cont;

	for (std::string l; std::getline(in, l);) {
		cont.push_back(l);
	}

	in.close();

	return cont;
}

std::vector<GLfloat> rdAttr(std::string fName, unsigned int attr) {
	const std::string id[3] = {
		"v",
		"vt",
		"vn"
	};
	const unsigned int sz[3] = {
		3,
		2,
		3
	};

	std::vector<std::string> buff = rd("res/" + fName + ".obj");

	std::vector<GLfloat> cont;
	for (int l = 0; l < buff.size(); l++) {
		std::vector<std::string> tok = split(buff[l], ' ');
		if (tok[0] == id[attr]) {
			for (int i = 1; i < 1 + sz[attr]; i++) {
				std::stringstream out;
				out << std::fixed << std::setprecision(4) << std::stof(tok[i]);

				cont.push_back(std::stof(out.str()));
			}
		}
	}

	return cont;
}

std::vector<GLushort> rdIdc(std::string fName, unsigned int attr) {
	std::vector<std::string> buff = rd("res/" + fName + ".obj");

	std::vector<GLushort> cont;
	for (int l = 0; l < buff.size(); l++) {
		std::vector<std::string> tok = split(buff[l], ' ');

		if (tok[0] == "f") {
			for (int i = 1; i < 1 + 3; i++) {
				std::vector<std::string> type = split(tok[i], '/');

				cont.push_back(std::stoi(type[attr]) - 1);
			}
		}
	}

	return cont;
}

class Obj {
	private:
		GLuint _vao;
		GLuint _vbo;

		Prog _prog;

		unsigned int _noIdc;

	public:
		Obj(GLfloat* vtc, GLushort* idc, unsigned int noIdc, glm::vec3 loc = glm::vec3(0.0)) :
			_prog("shad", "shad"),
			_noIdc(noIdc) {
				// data
				glGenVertexArrays(1, &_vao);
				glBindVertexArray(_vao);

				// position
				glGenBuffers(1, &_vbo);
				glBindBuffer(GL_ARRAY_BUFFER, _vbo);
				glBufferData(GL_ARRAY_BUFFER, _noIdc * 3 * sizeof (GLfloat), vtc, GL_STATIC_DRAW);

				// index
				GLuint ibo;
				glGenBuffers(1, &ibo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, _noIdc * sizeof (GLushort), idc, GL_STATIC_DRAW);

				// matrix
				glm::mat4 model = glm::mat4(1.0);
				model = glm::translate(model, loc);

				glm::mat4 view = glm::lookAt(glm::vec3(15, 15, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				glm::mat4 proj = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);

				_prog.use();

				/// attribute
				GLint attrPos = glGetAttribLocation(_prog._id, "pos");
				glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
				glEnableVertexAttribArray(attrPos);

				/// uniform
				GLint uniModel = glGetUniformLocation(_prog._id, "model");
				GLint uniView = glGetUniformLocation(_prog._id, "view");
				GLint uniProj = glGetUniformLocation(_prog._id, "proj");

				glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

				_prog.unUse();
			}

		void draw() {
			glBindVertexArray(_vao);
			_prog.use();

			glDrawElements(GL_TRIANGLES, _noIdc, GL_UNSIGNED_SHORT, (GLvoid*) 0);

			_prog.unUse();
			glBindVertexArray(0);
		}
};

int main() {
	Disp disp("asdf", 800, 600);

	// data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vtcBg[3 * 2 * 2] = {
		-1.0, -1.0,
		1.0, -1.0,
		-1.0, 1.0,

		-1.0, 1.0,
		1.0, -1.0,
		1.0, 1.0
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof vtcBg, vtcBg, GL_STATIC_DRAW);

	// shader
	Prog prog("scr", "gradient");

	prog.use();

	/// attribute
	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	prog.unUse();

	GLfloat vtc[2 * 2 * 2 * 3];
	int i = 0;
	for (int z = 0; z < 2; z++) {
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 2; x++) {
				vtc[i] = x ? 1 : -1;
				vtc[i + 1] = y ? 1 : -1;
				vtc[i + 2] = z ? 1 : -1;

				i += 3;
			}
		}
	}

	GLushort idc[3 * 2 * 3 * 2] = {
		0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		0, 4, 1,
		1, 4, 5,

		2, 6, 3,
		3, 6, 7,

		0, 4, 2,
		2, 4, 6,

		1, 5, 3,
		3, 5, 7
	};

	Obj cube(vtc, idc, sizeof idc / sizeof *idc);

	std::vector<GLfloat> vtcTeapot = rdAttr("teapot", 0);
	std::vector<GLushort> idcTeapot = rdIdc("teapot", 0);
	Obj teapot(&vtcTeapot[0], &idcTeapot[0], idcTeapot.size(), glm::vec3(3.0, 7.0, 0.0));

	std::vector<GLfloat> vtcTorus = rdAttr("Torus", 0);
	std::vector<GLushort> idcTorus = rdIdc("Torus", 0);
	Obj torus(&vtcTorus[0], &idcTorus[0], idcTorus.size(), glm::vec3(0.0, 5.0, 0.0));

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		disp.clear(0, 0, 0, 1);

		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(vao);
		prog.use();

		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

		prog.unUse();
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);

		cube.draw();
		teapot.draw();
		torus.draw();

		disp.update();
	}
}
