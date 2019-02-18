#include "AICar.h"
#include "Utility.h"

AICar::AICar(const char* meshName, Primitive* primitive, unsigned int texID, DRAW_MODE drawMode) :
	Car(meshName, primitive, texID, drawMode){


	obb->setHalf(Vector3(2.192, 1.2445, 4.289));
	defaultObb->setHalf(Vector3(2.192, 1.2445, 4.289));
	loadWaypoints();

	position = waypoints[0]->getPos();
	currentID = 0;
	rotation.y = waypoints[0]->getRot();
	getInputs();
	
	//Vector3 targetDir = (current->getPos() - position);
	//std::cout << "Angle: " << acos(targetDir.Dot(forward) * (1.0f / (targetDir.Length() * forward.Length())));
	//rotation.y = acos(targetDir.Dot(forward) * (1.0f / (targetDir.Length() * forward.Length())));
}

AICar::AICar()
{
}


AICar::~AICar()
{
}

void AICar::Update(double dt) {


	//if (currentID + 1 < waypoints.size())
	//{
	//	AIWaypoint* target = waypoints[currentID + 1];
	//	Vector3 targetPos = target->getPos();
	//	Vector3 targetRot = Vector3(0, target->getRot(), 0);
	//	float distance = (targetPos - position).Length();

	//	
	//	//
	//	//float tolerance = (target->getPos() - waypoints[currentID]->getPos()).Length() * 0.5f;



	//	velocity += updatePosition(-currentAccInput, -currentSteerInput, dt);
	//
	//	if (distance <= 1.0f)
	//	{
	//		currentID++;
	//		getInputs();
	//	}
	//	//Vector3 forecastPos = position + velocity;
	//	//if ((currentAccInput != 0 && (target->getPos() - forecastPos).Length() <= tolerance)) {
	//	//	//velocity = Utility::Lerp(velocity, 0.0, 5.0 * dt);
	//	//	//if (velocity.Length() < 0.1f) velocity.SetZero();
	//	//	currentID++;
	//	//	getInputs();
	//	//}
	//	//else if(currentSteerInput != 0 && (target->getRot() + currentSteer) <= 2.0f) {
	//	//	currentID++;
	//	//	getInputs();
	//	//}
	//	//else if (currentSteerInput == 0 && currentAccInput == 0 && (target->getPos() - forecastPos).Length() <= tolerance) {
	//	//	currentID++;
	//	//	getInputs();
	//	//}
	//	//else {
	//	//	currentAccInput = 1;
	//	//}

	//}
	Mesh::Update(dt);
}

void AICar::getInputs() {
	currentAccInput = waypoints[currentID]->getAccInput();
	currentSteerInput = waypoints[currentID]->getSteerInput();

	//if (currentAccInput == 100.0f) {
	//	if (currentID == 0)
	//		currentAccInput = 0;
	//	else
	//		currentAccInput = waypoints[currentID - 1]->getAccInput();
	//}
	//if (currentSteerInput == 100.0f) {
	//	if (currentID == 0)
	//		currentSteerInput = 0;
	//	else
	//		currentSteerInput = waypoints[currentID - 1]->getSteerInput();
	//}

}


void AICar::loadWaypoints() {
	std::ifstream handle("Data//waypoints.txt");
	
	if (!handle.is_open())
		std::cout << "[AICar] Could not load waypoints!" << std::endl;

	std::string line;
	while (std::getline(handle, line)) {
		std::vector<std::string> args = Utility::splitLine(line, '=');
		if (args[0] == "waypoint") {
			std::vector<std::string> params = Utility::splitLine(args[1], ',');
			Vector3 p;
			float accInput = 100.0f;
			float steerInput = 100.0f;
			float rot = -1.0f;
			for (int i = 0; i < params.size(); i++) {
				std::vector<std::string> attrib = Utility::splitLine(params[i], ':');
				if (attrib[0] == "pos") {
					std::string pos = attrib[1].substr(1, attrib[1].length() - 2);
					std::vector<std::string> positions = Utility::splitLine(pos, ' ');
					p.Set(std::stof(positions[0]), std::stof(positions[1]), std::stof(positions[2]));
				}
				else if (attrib[0] == "rot") {
					rot = std::stof(attrib[1]);
				}
				else if (attrib[0] == "steer") {
					steerInput = std::stof(attrib[1]);
				}
				else if (attrib[0] == "acc") {
					accInput = std::stof(attrib[1]);
				}
			}
			waypoints.push_back(new AIWaypoint(p, rot, accInput, steerInput, 1.0f));
		}
	}

	handle.close();
}