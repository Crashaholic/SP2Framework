#ifndef UTILITY_H
#define UTILITY_H

#include "Vertex.h"
#include "Mtx44.h"
#include <vector>
#include <string>
#include <sstream>

Position operator*(const Mtx44& lhs, const Position& rhs);

class Utility
{
public:
	Utility();
	~Utility();
	static Vector3 rotatePointByX(Vector3 pos, float angle);
	static Vector3 rotatePointByY(Vector3 pos, float angle);
	static Vector3 rotatePointByZ(Vector3 pos, float angle);
	static Vector3 sLerp(Vector3 start, Vector3 end, float percent);
	static Vector3 Lerp(Vector3 start, Vector3 end, float percent);
	static std::vector<std::string> splitLine(const std::string& s, char delimiter);
};

#endif