#ifndef OVERLAY_H
#define OVERLAY_H

#include "Texture.h"
#include <vector>



class Overlay
{
public:
	Overlay();
	~Overlay();
	//void addTexture(Vector3 position, Vector3 scale, const char *filePath, int dontFlip);
	//void addTexture(Vector3 position, Vector3 scale, unsigned int id, int dontFlip);
	//void render();
	//void renderTexture(int pos);
private:
	std::vector<Texture*> textures;
	unsigned int VBO;

};

#endif