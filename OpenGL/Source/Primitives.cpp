#include "Primitives.h"
#include "Utility.h"

bool findExistingVertex(std::map<Vertex, unsigned int>& data, Vertex& vertex, unsigned int& index);

Primitives::Primitives()
{
}


Primitives::~Primitives()
{
}



Primitive* Primitives::loadModel(const char* path, bool generateCollider)
{
	std::ifstream handle(path);

	if (!handle)
	{
		std::cout << "Error: Model at " << path << " cannot be loaded!" << std::endl;
		return new Primitive();
	}


	// Stores actual data about each vertex
	std::vector<TexCoord> texCoords;
	std::vector<Vector3> normals;
	std::vector<Position> positions;

	// Stores the index of each vertex (position, texCoord and normal) of each triangle 
	// where index = position+1
	// Index is the position of the specific attribute in its respective vector, but incremented by 1 
	// since vector starts from 0 while obj file starts from 1
	std::vector<unsigned int> vertIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;

	// Output into vertex buffer for drawing
	std::map<Vertex, unsigned int> data;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::string line;

	Vector3 min, max = Vector3(0, 0, 0);

	while (std::getline(handle, line))
	{
		if (line[0] != 'v' && line[0] != 'f' && line[0] != 's') continue;
		
		std::vector<std::string> args = Utility::splitLine(line, ' ');

		// Tex Coord ==> vt X Y Z
		if (line[1] == 't')
		{
			texCoords.push_back(TexCoord(std::stof(args[1]), std::stof(args[2])));
		}
		// Normal ==> vn X Y Z
		else if (line[1] == 'n')
		{
			normals.push_back(Vector3(std::stof(args[1]), std::stof(args[2]), std::stof(args[3])));
		}
		// Vertex ==> v X Y Z
		else if (line[0] == 'v')
		{
			float x = std::stof(args[1]);
			float y = std::stof(args[2]);
			float z = std::stof(args[3]);
			if (positions.size() != 0) {
				if (x < min.x)
					min.x = x;
				else if (x > max.x)
					max.x = x;
				else if (y < min.y)
					min.y = y;
				else if (y > max.y)
					max.y = y;
				else if (z < min.z)
					min.z = z;
				else if (z > max.z)
					max.z = z;
			}
			else {
				min.Set(x, y, z);
				max.Set(x, y, z);
			}
			positions.push_back(Position(x, y, z));
		}
		
		else if (line[0] == 'f')
		{
			if (args.size() > 5)
			{
				std::cout << "Error: Incorrect format for face in " << path << "! " << args.size()-1 << "-sided polygons are not supported!" << std::endl;
				return new Primitive();
			}

			// Face ==> f v1/uv1/n1 v2/uv2/n2 v3/uv3/n3 (v4/uv4/n4)..
			// General Case - be it for triangles or quad
			// Triangle 1 -> v1, v2, v3
			for (int i = 1; i <= 3; i++)
			{
				// Format ==> v/vt/n
				std::vector<std::string> indiv = Utility::splitLine(args[i], '/');

				vertIndices.push_back(std::stoul(indiv[0]));
				if (indiv[1] == "")
					uvIndices.push_back(0);
				else
					uvIndices.push_back(std::stoul(indiv[1]));
				normalIndices.push_back(std::stoul(indiv[2]));
			}

			// Quad -> 2 Triangles
			// Triangle 2 -> v1, v3, v4
			if (int(args.size()) - 1 == 4)
			{
				// Triangle 2
				for (int i = 1; i <= (int) (args.size() - 1); i++)
				{
					// Skip v2
					if (i == 2) continue;

					// Format ==> v/vt/n
					std::vector<std::string> indiv = Utility::splitLine(args[i], '/');
					vertIndices.push_back(std::stoul(indiv[0]));
					if (indiv[1] == "")
						uvIndices.push_back(0);
					else
						uvIndices.push_back(std::stoul(indiv[1]));
					normalIndices.push_back(std::stoul(indiv[2]));
				}
			}

		}
	}

	

	// Loop through all the indices
	for (unsigned int i = 0; i < (unsigned int)vertIndices.size(); i++)
	{
		unsigned int vertIndex = vertIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normIndex = normalIndices[i];

		// Index starts from 1 in obj file
		// Therefore, actual position in vector = Index - 1
		Position pos = positions[vertIndex - 1];
		TexCoord uv = texCoords[uvIndex - 1];
		Vector3 norm = normals[normIndex - 1];

		Vertex v;
		v.pos = pos;
		v.texCoord = uv;
		v.normal = norm;
		v.color.Set(1, 1, 1);

		unsigned int index;
		if (findExistingVertex(data, v, index))
		{
			indices.push_back(index);
		}
		else
		{
			vertices.push_back(v);
			unsigned int newIndex = ((unsigned int)vertices.size()) - 1;
			indices.push_back(newIndex);
			data[v] = newIndex;
		}
	}

	return new Primitive(vertices, indices, min, max);
}

bool findExistingVertex(std::map<Vertex, unsigned int>& data, Vertex& vertex, unsigned int& index)
{
	if (data.find(vertex) == data.end())
	{
		return false;
	}
	else
	{
		index = data.at(vertex);
		return true;
	}
}



Primitive* Primitives::generateAxes()
{
	std::vector<Vertex> vertices;

	Vertex v;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertices.push_back(v);

	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertices.push_back(v);

	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertices.push_back(v);

	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertices.push_back(v);

	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertices.push_back(v);

	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertices.push_back(v);

	std::vector<unsigned int> indices = { 0,1,2,3,4,5 };

	return new Primitive(vertices, indices, Vector3(0,0,0), Vector3(0,0,0));
}

Primitive* Primitives::generateQuad(Color color)
{
	std::vector<Vertex> vertices;

	Vertex v;
	v.color = color;
	v.normal.Set(0, 0, 1);

	v.pos.Set(-0.5, 0.5, 0);
	v.texCoord.Set(0, 1);
	vertices.push_back(v);

	v.pos.Set(-0.5, -0.5, 0);
	v.texCoord.Set(0, 0);
	vertices.push_back(v);


	v.pos.Set(0.5, 0.5, 0);
	v.texCoord.Set(1, 1);
	vertices.push_back(v);


	v.pos.Set(0.5, -0.5, 0);
	v.texCoord.Set(1, 0);
	vertices.push_back(v);

	std::vector<unsigned int> indices = { 0,1,2,2,1,3 };

	return new Primitive(vertices, indices, Vector3(-0.5, -0.5, 0), Vector3(0.5, 0.5, 0));

}

