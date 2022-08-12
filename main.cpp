#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "disp.h"
#include "prog.h"
#include "util.h"

class Obj {
	private:
		GLuint _vao;
		GLuint _vbo;

		Prog _prog;

		unsigned int _noIdc;

	public:
		Obj() :
			_prog("shad", "shad") {
				// data
				glGenVertexArrays(1, &_vao);
				glBindVertexArray(_vao);

				// position
				glGenBuffers(1, &_vbo);
				glBindBuffer(GL_ARRAY_BUFFER, _vbo);

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
				glBufferData(GL_ARRAY_BUFFER, sizeof vtc, vtc, GL_STATIC_DRAW);

				// index
				GLuint ibo;
				glGenBuffers(1, &ibo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

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
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof idc, idc, GL_STATIC_DRAW);

				_noIdc = sizeof idc / sizeof *idc;

				// matrix
				glm::mat4 model = glm::mat4(1.0);
				glm::mat4 view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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

	Obj cube;

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		disp.clear(0, 0, 0, 1);

		cube.draw();

		disp.update();
	}
}
