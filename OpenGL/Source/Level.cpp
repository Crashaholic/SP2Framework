#include "Level.h"
#include "Manager.h"
#include "Player.h"
#include "LevitationPad.h"
#include "AICar.h"


Level::Level(const char* levelPath)
{
	currentScreen = "";
	Mtx44 proj;
	proj.SetToPerspective(45.0f, (float)Application::winWidth / (float)Application::winHeight, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);
	Load(levelPath);
	
}

void Level::setScreen(std::string screen) {
	currentScreen = screen;
}

void Level::Load(std::string path) {

	tree = new QuadTree(Vector3(-1000, 0, -1000), Vector3(1000, 0, 1000));
	std::ifstream handle(path);
	if (!handle.is_open()) {
		std::cout << "[Error] Could not load level.txt!" << std::endl;
	}

	std::string line;
	struct Object {
		std::map<std::string, std::string> values;
		void Set(std::string key, std::string value)
		{
			values[key] = value;
		}

		std::string Get(std::string key)
		{
			return values[key];
		}
	};

	std::map<std::string, std::vector<Object*>> collection;
	std::string category = "";
	Object* current = nullptr;
	while (std::getline(handle, line))
	{
		std::vector<std::string> args = Utility::splitLine(line, '=');

		if (Utility::startsWith(line, " ") || line == "")
		{
			continue;
		}
		else if (Utility::startsWith(line, "category")) {
			category = args[1];
			current = nullptr;
			continue;
		}
		else if (Utility::startsWith(line, "name"))
		{
			current = new Object();
			collection[category].push_back(current);
		}
		current->Set(args[0], args[1]);

	}

	for (auto& c : collection) {

		if (c.first == "objects") {
			std::vector<Object*>* objs = &c.second;
			for (int i = 0; i < (int)objs->size(); i++) {
				current = objs->at(i);
				bool collision = (current->Get("collision") == "true" ? true : false);
				bool gravity = (current->Get("gravity") == "true" ? true : false);
				unsigned int textureID = LoadTGA(current->Get("texture").c_str());
				Primitive* primitive = Primitives::loadModel(current->Get("model").c_str());

				std::string type = current->Get("type");
				Mesh* m = nullptr;
				if (type == "player") {
					m = new Player(current->Get("name").c_str(), primitive, textureID);
				}
				else if (type == "car") {
					m = new Car(current->Get("name").c_str(), primitive, textureID);
				}
				else if (type == "pad") {
					m = new LevitationPad(current->Get("name").c_str(), primitive, textureID, std::stof(current->Get("levitation")));
				}
				else if (type == "ai") {
					m = new AICar(current->Get("name").c_str(), primitive, textureID);
				}
				else {
					m = new Mesh(current->Get("name").c_str(), primitive, textureID, collision, gravity, type);
				}
				std::vector<std::string> pos = Utility::splitLine(current->Get("position"), ',');
				std::vector<std::string> rot = Utility::splitLine(current->Get("rotation"), ',');
				m->position = Vector3(std::stof(pos[0]), std::stof(pos[1]), std::stof(pos[2]));
				m->rotation = Vector3(std::stof(rot[0]), std::stof(rot[1]), std::stof(rot[2]));
				m->Init();
				objects[current->Get("name")] = m;
				if(collision)
					tree->Insert(m);
			}
		}
		else if (c.first == "ui") {
			std::vector<Object*>* objs = &c.second;
			for (int i = 0; i < (int)objs->size(); i++) {
				current = objs->at(i);

				std::string type = current->Get("type");

				if (type == "button") {
					unsigned int textureID = LoadTGA(current->Get("texture").c_str());
					unsigned int hoverID = LoadTGA(current->Get("hovertexture").c_str());
					std::vector<std::string> pos = Utility::splitLine(current->Get("position"), ',');
					Vector3 position = Vector3(std::stof(pos[0]), std::stof(pos[1]), 0);
					float rot = std::stof(current->Get("rotation"));
					std::vector<std::string> scale = Utility::splitLine(current->Get("scale"), ',');
					Vector3 scal = Vector3(std::stof(scale[0]), std::stof(scale[1]), 0);
					std::string action = current->Get("action");

					std::string screen = current->Get("screen");

					if (screens.find(screen) == screens.end()) {
						screens[screen] = new GUIScreen();
					}

					screens[screen]->addButton(new GUIButton(position, rot, scal, textureID, hoverID, action));
				}




			}
		}
	}

	handle.close();


}

void Level::renderGui()
{
	screens[currentScreen]->render();
}

void Level::renderMesh()
{
	ShaderProgram* lit = Manager::getInstance()->getShader("lit");
	modelStack.LoadIdentity();

	Player* player = dynamic_cast<Player*>(objects["human"]);
	viewStack.LoadMatrix(player->getCamera()->LookAt());


	for (auto& obj : objects)
	{
		
		Mesh* m = obj.second;
		std::string key = obj.first;
		if (Utility::startsWith(key, "skybox") || key == "axes" || key == "playerAxes") continue;

		if (obj.first == "ai" || obj.first == "car") {
			glDisable(GL_CULL_FACE);
		}

		modelStack.PushMatrix();
		m->ResetOBB();
		m->Translate(modelStack, m->position.x, m->position.y, m->position.z);
		m->Rotate(modelStack, m->rotation.x, 1, 0, 0);
		m->Rotate(modelStack, m->rotation.y, 0, 1, 0);
		m->Rotate(modelStack, m->rotation.z, 0, 0, 1);
		Mtx44 MVP, modelView, modelView_inverse_tranpose;
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		modelView = viewStack.Top() * modelStack.Top();

		lit->use();

		bool enableLight = true;
		if (enableLight)
		{
			modelView_inverse_tranpose = modelView.GetInverse().GetTranspose();
			lit->setUniform("lightEnabled", 1);
			lit->setUniform("MV_inverse_transpose", modelView_inverse_tranpose.a);
			lit->setUniform("material.kAmbient", m->material.kAmbient.r, m->material.kAmbient.g, m->material.kAmbient.b);
			lit->setUniform("material.kDiffuse", m->material.kDiffuse.r, m->material.kDiffuse.g, m->material.kDiffuse.b);
			lit->setUniform("material.kSpecular", m->material.kSpecular.r, m->material.kSpecular.g, m->material.kSpecular.b);
			lit->setUniform("material.kShininess", m->material.kShininess);

		}
		else
		{
			lit->setUniform("lightEnabled", 0);
		}

		// Do normal one texture per mesh bind
		if (m->textureID > 0)
		{
			lit->setUniform("colorTextureEnabled", 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m->textureID);
			lit->setUniform("colorTexture", 0);
		}
		else
		{
			lit->setUniform("colorTextureEnabled", 0);
		}

		lit->updateUniforms();

		m->Render(modelStack, viewStack, projectionStack, lit);

		if (m->textureID > 0)
			glBindTexture(GL_TEXTURE_2D, 0);
		modelStack.PopMatrix();

		if (obj.first == "ai" || obj.first == "car") {
			glEnable(GL_CULL_FACE);
		}
	}
}

void Level::Render()
{
	glEnable(GL_DEPTH_TEST);
	renderMesh();
	glDisable(GL_DEPTH_TEST);
	renderGui();
	glEnable(GL_DEPTH_TEST);
}