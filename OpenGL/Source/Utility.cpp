#include "Utility.h"



Utility::Utility()
{
}


Utility::~Utility()
{
}



Position operator*(const Mtx44& lhs, const Position& rhs) {
	float b[4];
	for (int i = 0; i < 4; i++)
		b[i] = lhs.a[0 * 4 + i] * rhs.x + lhs.a[1 * 4 + i] * rhs.y + lhs.a[2 * 4 + i] * rhs.z + lhs.a[3 * 4 + i] * 1;
	return Position(b[0], b[1], b[2]);
}

Vector3 Utility::rotatePointByX(Vector3 pos, float angle) {
	float radX = Math::DegreeToRadian(-angle);
	Mtx44 xRot = Mtx44(1, 0, 0, 0,
		0, cos(radX), -sin(radX), 0,
		0, sin(radX), cos(radX), 0,
		0, 0, 0, 1);

	return xRot * pos;
}

Vector3 Utility::rotatePointByY(Vector3 pos, float angle) {
	float radY = Math::DegreeToRadian(angle);
	Mtx44 yRot = Mtx44(cos(radY), 0, -sin(radY), 0,
		0, 1, 0, 0,
		sin(radY), 0, cos(radY), 0,
		0, 0, 0, 1);

	return yRot * pos;
}


Vector3 Utility::rotatePointByZ(Vector3 pos, float angle) {
	float radZ = Math::DegreeToRadian(-angle);
	Mtx44 zRot = Mtx44(cos(radZ), -sin(radZ), 0, 0,
		sin(radZ), cos(radZ), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	return zRot * pos;
}


std::vector<std::string> Utility::splitLine(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
		tokens.push_back(token);
	return tokens;
}

