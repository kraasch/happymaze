
build:
	mkdir -p ./build
	g++ ./src/HappyMaze.cpp -o ./build/HappyMaze -lncurses && ./build/HappyMaze

clean:
	rm -rf ./build

