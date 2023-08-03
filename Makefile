CC=g++
CFLAGS=-Wall -Wextra -std=c++20 -ggdb `pkg-config --cflags sfml-all`
LIBS=`pkg-config --libs sfml-all`
SRC=src/Config.cpp src/main.cpp src/Spline.cpp src/Path.cpp
path_editor: $(SRC)
	$(CC) -o path_editor $(SRC) $(CFLAGS) $(LIBS)	
