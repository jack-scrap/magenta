#pragma once

#include "util.h"

class Shad {
	public:
		GLuint _id;

		Shad(std::string name, bool vtx);
};

class Prog {
	public:
		GLuint _id;

		Prog(std::string vtx, std::string frag);

		void use();

		void unUse();
};
