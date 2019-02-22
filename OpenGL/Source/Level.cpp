#include "Level.h"
#include "Manager.h"
#include "Player.h"
#include "LevitationPad.h"
#include "AICar.h"
#include "Application.h"


Level::Level(const char* levelPath)
{
	Mtx44 proj;
	proj.SetToPerspective(45.0f, (float)Application::winWidth / (float)Application::winHeight, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(proj);
	Load(levelPath);

	for (int i = 0; i < 2; i++)
		if(lightSources.size() < 2)
			lightSources.push_back(new LightSource());
	
}



Level::~Level()
{
	delete tree;

	for (auto const& object : objects)
		if (object.second != nullptr)
			delete object.second;

	for (int i = 0; i < (int)lightSources.size(); i++)
		delete lightSources[i];
}

void Level::setScreen(std::string screen) {
	currentScreen = screen;
}


void Level::Load(std::string path) {

	tree = new QuadTree(Vector3(-1500, 0, -1500), Vector3(1500, 0, 1500));
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
			if (values.find(key) == values.end()) return "invalid";
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
		else if (Utility::startsWith(line, "defaultscreen")) {
			currentScreen = args[1];
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
					m = new Player(current->Get("name").c_str(), primitive, current->Get("input"), textureID);
				}
				else if (type == "car") {
					m = new Car(current->Get("name").c_str(), primitive, current->Get("input"), textureID);
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
				if (collision) {
					tree->Insert(m);
					Vector3 currentMin = m->position + Vector3(-m->getOBB()->getHalf().x, 0, -m->getOBB()->getHalf().z);
					Vector3 currentMax = m->position + Vector3(m->getOBB()->getHalf().x, 0, m->getOBB()->getHalf().z);
					currentMin = Utility::rotatePointByY(currentMin, m->rotation.y);
					currentMax = Utility::rotatePointByY(currentMax, m->rotation.y);
					if (tree->withinBounds(currentMax))
						tree->Insert(m, currentMax);
					if (tree->withinBounds(currentMin))
						tree->Insert(m, currentMin);
				}
			}
		}
		else if (c.first == "ui") {
			std::vector<Object*>* objs = &c.second;
			for (int i = 0; i < (int)objs->size(); i++) {
				current = objs->at(i);

				std::string type = current->Get("type");
				std::string screen = current->Get("screen");

				if (screens.find(screen) == screens.end()) {
					screens[screen] = new GUIScreen(screen);
				}

				std::vector<std::string> pos = Utility::splitLine(current->Get("position"), ',');
				Vector3 position = Vector3(std::stof(pos[0]), std::stof(pos[1]), 0);
				float rot = std::stof(current->Get("rotation"));
				std::vector<std::string> scale = Utility::splitLine(current->Get("scale"), ',');
				Vector3 scal = Vector3(std::stof(scale[0]), std::stof(scale[1]), 0);

				if (type == "button") {

		
					std::string action = current->Get("action");

					if (current->Get("texture") == "invalid")
					{
						std::vector<std::string> colors = Utility::splitLine(current->Get("color"), ',');
						Vector3 color = Vector3(std::stof(colors[0]), std::stof(colors[1]), std::stof(colors[2]));
						std::vector<std::string> hoverColors = Utility::splitLine(current->Get("hovercolor"), ',');
						Vector3 hoverColor = Vector3(std::stof(hoverColors[0]), std::stof(hoverColors[1]), std::stof(hoverColors[2]));
						float normalAlpha = std::stof(colors[3]);
						float hoverAlpha = std::stof(hoverColors[3]);
						screens[screen]->addButton(new GUIButton(position, rot, scal, color, normalAlpha, hoverColor, hoverAlpha, action));
					}
					else
					{
						unsigned int textureID = LoadTGA(current->Get("texture").c_str());
						unsigned int hoverID = LoadTGA(current->Get("hovertexture").c_str());
						screens[screen]->addButton(new GUIButton(position, rot, scal, textureID, hoverID, action));
					}

				}
				else if (type == "texture")
				{

					if (current->Get("texture") == "invalid")
					{
						std::vector<std::string> colors = Utility::splitLine(current->Get("color"), ',');
						Vector3 color = Vector3(std::stof(colors[0]), std::stof(colors[1]), std::stof(colors[2]));
						float normalAlpha = std::stof(colors[3]);
						screens[screen]->addTexture(new GUITexture(position, rot, scal, color, normalAlpha));
					}
					else
					{
						unsigned int textureID = LoadTGA(current->Get("texture").c_str());
						screens[screen]->addTexture(new GUITexture(position, rot, scal, textureID));
					}
				}




			}
		}
	}

	handle.close();


}



void Level::renderGUI()
{
	screens[currentScreen]->Render();
}

void Level::renderSkybox()
{
	if (Manager::getInstance()->getLevelName() != "game") return;

	ShaderProgram* lit = Manager::getInstance()->getShader("lit");
	lit->use();
	float length = 1000.0f;
	modelStack.PushMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, length / 2, 0.0f);
	modelStack.Rotate(-90.0f, 0, 1, 0);
	modelStack.Rotate(90.0f, 1, 0, 0);
	modelStack.Scale(length, length, 1.0f);
	renderMesh(objects["skyboxTop"]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, -length / 2, 0.0f);
	modelStack.Rotate(-90.0f, 1, 0, 0);
	modelStack.Scale(length, length, 1.0f);
	renderMesh(objects["skyboxBottom"]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, 0.0f, length / 2);
	modelStack.Rotate(180.0f, 1, 1, 0);
	modelStack.Rotate(-90.0f, 0, 0, 1);
	modelStack.Scale(length, length, 1.0f);
	renderMesh(objects["skyboxFront"]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.0f, 0.0f, -length / 2);
	modelStack.Scale(length, length, 1.0f);
	renderMesh(objects["skyboxBack"]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-length / 2, 0.0f, 0.0f);
	modelStack.Rotate(90.0f, 0, 1, 0);
	modelStack.Scale(length, length, 1.0f);
	renderMesh(objects["skyboxLeft"]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(length / 2, 0.0f, 0.0f);
	modelStack.Rotate(-90.0f, 0, 1, 0);
	modelStack.Scale(length, length, 1.0f);
	renderMesh(objects["skyboxRight"]);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Level::renderMesh(Mesh* mesh)
{
	Mtx44 MVP, modelView, modelView_inverse_tranpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	modelView = viewStack.Top() * modelStack.Top();
	ShaderProgram* lit = Manager::getInstance()->getShader("lit");
	lit->use();

	bool enableLight = true;
	if (enableLight)
	{
		modelView_inverse_tranpose = modelView.GetInverse().GetTranspose();
		lit->setUniform("lightEnabled", 1);
		lit->setUniform("MV_inverse_transpose", modelView_inverse_tranpose.a);
		lit->setUniform("material.kAmbient", mesh->material.kAmbient.r, mesh->material.kAmbient.g, mesh->material.kAmbient.b);
		lit->setUniform("material.kDiffuse", mesh->material.kDiffuse.r, mesh->material.kDiffuse.g, mesh->material.kDiffuse.b);
		lit->setUniform("material.kSpecular", mesh->material.kSpecular.r, mesh->material.kSpecular.g, mesh->material.kSpecular.b);
		lit->setUniform("material.kShininess", mesh->material.kShininess);

	}
	else
	{
		lit->setUniform("lightEnabled", 0);
	}

	// Do normal one texture per mesh bind
	if (mesh->textureID > 0)
	{
		lit->setUniform("colorTextureEnabled", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		lit->setUniform("colorTexture", 0);
	}
	else
	{
		lit->setUniform("colorTextureEnabled", 0);
	}

	lit->updateUniforms();

	mesh->Render(modelStack, viewStack, projectionStack, lit);
}

void Level::Render()
{
	ShaderProgram* lit = Manager::getInstance()->getShader("lit");
	modelStack.LoadIdentity();

	std::string levelName = Manager::getInstance()->getLevelName();

	glEnable(GL_DEPTH_TEST);
	if (levelName == "game")
	{
		modelStack.LoadIdentity();
		glViewport(0, 0, Application::winWidth, Application::winHeight / 2.0f);
		viewStack.LoadMatrix(dynamic_cast<Player*>(objects["player2"])->getCamera()->LookAt());

		for (int i = 0; i < (int)lightSources.size(); i++)
			lightSources.at(i)->updateAttributes(viewStack);

		renderObjects();

		modelStack.LoadIdentity();
		glViewport(0, Application::winHeight / 2.0f, Application::winWidth, Application::winHeight / 2.0f);
		viewStack.LoadMatrix(dynamic_cast<Player*>(objects["player"])->getCamera()->LookAt());

		for (int i = 0; i < (int)lightSources.size(); i++)
			lightSources.at(i)->updateAttributes(viewStack);

		renderObjects();
	}

	glViewport(0, 0, Application::winWidth, Application::winHeight);

	glDisable(GL_DEPTH_TEST);
	renderGUI();
	glEnable(GL_DEPTH_TEST);
}

void Level::renderObjects()
{
	
	renderSkybox();

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


		renderMesh(m);

		if (m->textureID > 0)
			glBindTexture(GL_TEXTURE_2D, 0);
		modelStack.PopMatrix();

		if (obj.first == "ai" || obj.first == "car") {
			glEnable(GL_CULL_FACE);
		}
	}
}

void Level::Update(double dt)
{
	for (auto& object : objects)
	{
		object.second->Update(dt);
	}

	screens[currentScreen]->Update();
}


void Level::spawnObject(Mesh* m)
{
	objects[m->name] = m;
	if (m->collisionEnabled)
		tree->Insert(m);
}

GUIScreen* Level::getScreen()
{
	if (screens.size() == 0) screens[currentScreen] = new GUIScreen(std::to_string(screens.size()));
	return screens[currentScreen];
}

QuadTree* Level::getTree()
{
	return tree;
}

Mesh* Level::getObject(std::string name)
{
	return objects[name];
}

std::vector<LightSource*>* Level::getLightSources()
{
	return &lightSources;
}

std::map<std::string, Mesh*>* Level::getObjects()
{
	return &objects;
}


std::string Level::getScreenName() {
	return currentScreen;
}