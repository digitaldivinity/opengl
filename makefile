test: test.cpp input.h camera.h
	g++ test.cpp -o test -fopenmp -lm -lGL -lGLU -lglut
double: double.cpp
	g++ double.cpp -o double -lGL -lGLU -lglut
snow: snow.cpp
	g++ snow.cpp -o snow -lGL -lGLU -lglut
