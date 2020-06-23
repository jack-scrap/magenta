#pragma once

#include "util.h"

class Shad {
	public:
		GLint _id;

		Shad(std::string name, bool vtx);
};

class Prog {
	public:
		GLint _id;

		Prog(std::string vtx, std::string frag);

		void use();

		void unUse();
};
