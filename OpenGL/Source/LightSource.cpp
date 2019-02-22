#include "LightSource.h"
#include "GL/glew.h"
#include "Light.h"
#include "shader.hpp"
#include "Manager.h"

int LightSource::lightCount = 0;



LightSource::~LightSource()
{
	lightCount--;
}

LightSource::LightSource()
{		
	UID = lightCount++;
}


void LightSource::setProperties() {
	ShaderProgram* shader = Manager::getInstance()->getShader("lit");
	shader->use();

	if (UID == 0) {

		light.type = Light::LIGHT_POINT;
		light.position.Set(1.0f, 1.0f, 1.0f);
		light.color.Set(1, 1, 1);
		light.power = 1.0f;
		light.kC = 1.0f;
		light.kL = 0.01f;
		light.kQ = 0.001f;
		light.exponent = 1.0f;
		light.cosCutoff = cos(Math::DegreeToRadian(45));
		light.cosInner = cos(Math::DegreeToRadian(30));
		light.spotDirection.Set(0.0f, 1.0f, 0.0f);
	}
	else if (UID == 1)
	{
		light.position.Set(4.5f, 18.0f, 0.0f);
		light.type = Light::LIGHT_SPOT;
		light.color.Set(1, 1, 0);
		light.power = 2.0f;
		light.kC = 1.0f;
		light.kL = 0.01f;
		light.kQ = 0.001f;
		light.exponent = 1.0f;
		light.cosCutoff = cos(Math::DegreeToRadian(30));
		light.cosInner = cos(Math::DegreeToRadian(15));
		light.spotDirection.Set(-0.3f, 1.0f, 0.0f);
	}



	shader->setUniform(getPropertyName("type"), light.type);
	shader->setUniform(getPropertyName("color"), light.color.r, light.color.g, light.color.b);
	shader->setUniform(getPropertyName("power"), light.power);
	shader->setUniform(getPropertyName("kC"), light.kC);
	shader->setUniform(getPropertyName("kL"), light.kL);
	shader->setUniform(getPropertyName("kQ"), light.kQ);
	shader->setUniform(getPropertyName("spotDirection"), light.spotDirection);
	shader->setUniform(getPropertyName("cosCutoff"), light.cosCutoff);
	shader->setUniform(getPropertyName("cosInner"), light.cosInner);
	shader->setUniform(getPropertyName("exponent"), light.exponent);
	shader->updateUniforms();
}

void LightSource::setPower(float power) {
	light.power = power;
}

std::string LightSource::getPropertyName(std::string prop) {
	return "lights[" + std::to_string(UID) + "]." + prop;
}

void LightSource::setSpotDirection(Vector3 spot) {
	light.spotDirection.Set(spot.x, spot.y, spot.z);
}

void LightSource::setPosition(Vector3 pos) {
	light.position = Position(pos.x, pos.y, pos.z);
}


void LightSource::updateAttributes(MS& viewStack) {

	ShaderProgram* shader = Manager::getInstance()->getShader("lit");
	shader->use();

	shader->setUniform(getPropertyName("power"), light.power);

	if (light.type == Light::LIGHT_DIRECTIONAL)
	{
		
		Vector3 lightDir(light.position.x, light.position.y, light.position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		shader->setUniform(getPropertyName("position_cameraspace"), lightDirection_cameraspace);

	}
	else if (light.type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light.position;
		Vector3 spotDirection_cameraspace = viewStack.Top() * light.spotDirection;

		shader->setUniform(getPropertyName("position_cameraspace"), lightPosition_cameraspace.x, lightPosition_cameraspace.y, lightPosition_cameraspace.z);
		shader->setUniform(getPropertyName("spotDirection"), spotDirection_cameraspace);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light.position;
		shader->setUniform(getPropertyName("position_cameraspace"), lightPosition_cameraspace.x, lightPosition_cameraspace.y, lightPosition_cameraspace.z);

	}
	shader->updateUniforms();
}