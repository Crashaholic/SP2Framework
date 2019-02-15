#ifndef LOAD_TGA_H
#define LOAD_TGA_H

unsigned LoadTGA(const char *file_path);

struct image
{
	unsigned int ID;
	unsigned int width, height;
};

#endif