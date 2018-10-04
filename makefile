test: test.cpp input.h
	g++ test.cpp -o test -lGL -lglut
run: test
	./test
