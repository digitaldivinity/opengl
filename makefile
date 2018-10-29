test: test.cpp input.h camera.h bmpparser.h
	g++ test.cpp -o test -fopenmp -lm -lGL -lGLU -lglut
run: test
	./test
