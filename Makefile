
#########################################################
#                  pirateGame Makefile                  #
#       Authors:  Alexandar Savic and Audrey Yang       #
#				Date:	  25 June 2022		   		  	#
#########################################################
###
###
### 
### 
# clang++ main.cpp -framework IOKit -framework Cocoa -framework OpenGL `pkg-config --libs --cflags raylib` -o main

#flags
CXX = clang++
CXXFLAGS = -framework IOKit -framework Cocoa -framework OpenGl 'pkg-config --libs --cflags raylib'
LDFLAGS = -g3

# This rule builds and links treeTraversal executable
pirateGame: main.o 
	$(CXX) $(CXXFLAGS) -o pirateGame $^

# This rule builds main.o
main.o: main.cpp
	$(CXX) main.cpp

# Clean rule
clean:
	rm *.o *~ a.out