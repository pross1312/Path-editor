CC=g++
CFLAGS=-Wall -Wextra -std=c++20 `pkg-config --cflags sfml-all`
LIBS=`pkg-config --libs sfml-all`
SRC=src/Config.cpp src/CubicCurve.cpp src/main.cpp src/Path.cpp src/Spline.cpp
default:
	$(CC) -o path_editor $(SRC) $(CFLAGS) $(LIBS)	
