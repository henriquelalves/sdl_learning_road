# Which files to compile as part of the object
OBJS = src/main.cpp

# Compiler used
CC = g++

# Additional compilation options
COMPILER_FLAGS = -w

# Libraries linked against
LINKER_FLAGS = -lSDL2

# Name of executable
OBJ_NAME = bin/main

# Compilation target
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
