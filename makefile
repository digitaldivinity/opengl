test: test.cpp
	g++ test.cpp -o test -lGL -lglut
run: test
	./test
