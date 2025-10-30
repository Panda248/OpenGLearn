#ifndef SHADERSTRUCT_H
#define SHADERSTRUCT_H	

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//Attempt at restructuring shader class to fit a struct. Trying to utilize more c code for this. Dunno if I'm really using more c code

typedef struct shader_struct {
	char* vertex_src, fragment_src;
	
} shader_t;

#endif
