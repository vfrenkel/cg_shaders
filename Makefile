UNAME := $(shell uname)

MacGLFramework = -framework Carbon -framework OpenGL -framework GLUT
LinuxGLFramework = -lglut -lGLU -lGL -lGLEW -lSOIL
GLFramework = NOTYETSET

Include = -I/usr/include/GL -I/usr/include -I src
Link = -L/usr/local/lib -lm -Llib
Flags = -g -o
Defines = -DEIGEN_DONT_VECTORIZE -DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
Sources = src/*.cpp

CC = g++
ExeName = pa3

# Define proper GL framework
ifeq ($(UNAME), Linux)
	GLFramework = $(LinuxGLFramework)
endif
ifeq ($(UNAME), Darwin)
	GLFramework = $(MacGLFramework)
endif

all: main

# Compiling
main:
	$(CC) $(Flags) $(ExeName) $(Defines) $(Include) $(Link) $(Sources) $(GLFramework)
	./$(ExeName)



clean:
	rm -f $(ExeName) src/*~
