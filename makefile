all:
	g++ lol2cpp.cpp -o lol2cpp

run: all
	./lol2cpp
