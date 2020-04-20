cc = g++ 

v = -std=c++17

gl = -lGLEW -lGL
sdl = -lSDL2

f = main.cpp prog.cpp util.cpp

make:
	$(cc) $(f) $(v) $(gl) $(sdl)
