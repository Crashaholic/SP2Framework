#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "MatrixStack.h"
#include "Utility.h"
#include "Light.h"
#include <string>


class LightSource
{
public:
	LightSource(int type, Vector3 position, Vector3 color, float power, float exponent, Vector3 spotDir);
	LightSource();
	~LightSource();
	static int lightCount;
	void setProperties();
	void updateAttributes(MS& viewStack);
	void resetAttributes();
	void setPower(float f);
	void setSpotDirection(Vector3 spot);
	void setPosition(Vector3 pos);
	Light light;
private:
	std::string getPropertyName(std::string prop);

	unsigned int UID;
	unsigned shader;
};

#endif