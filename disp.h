#include <string>
#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

class Disp {
	private:
		SDL_Window* win;

		SDL_GLContext ctx;

	public:
		bool open = false;

		Disp(const char* title, int wd, int ht) {
			SDL_Init(SDL_INIT_EVERYTHING);

			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

			SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wd, ht, SDL_WINDOW_OPENGL);

			ctx = SDL_GL_CreateContext(win);

			GLenum status = glewInit();
			if (status != GLEW_OK) {
				std::cerr << "Glew failed to initialize" << std::endl;
			}

			open = true;

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		void clear(float r, float g, float b, float a) {
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void update() {
			SDL_GL_SwapWindow(win);
		}

		~Disp() {
			SDL_GL_DeleteContext(ctx);

			SDL_DestroyWindow(win);

			SDL_Quit();
		}
};
