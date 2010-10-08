#include "OBJLoader.h"
#include "FileManager.h"
#include <math.h>

using namespace std;
using namespace FileManager;

OBJLoader::OBJLoader() : shader_id(NULL), vbo_id(NULL)
{
	timer.Touch();
	name = "";
}

OBJLoader::~OBJLoader()
{
	Log::Print("Cleaning up mesh " + name);
}

void OBJLoader::Load(string file)
{
	file = GetFile(file);
	if (!FileExists(file))
	{
		Log::Print("File \"" + file + "\" not found.");
		return;
	}
	file = GetFileContents(file);

	// store OBJ stuff here before loading up the full mesh var.
	vector<MeshData> vertices;
	vector<MeshData> normals;
	vector<MeshData> coords;

	vector<string> data = Util::split(file, '\n');
	for (int i = 0; i<data.size(); i++)
	{
		string line = Util::chop(data[i], "#");
		if (line.empty())
			continue;
		vector<string> lvec;
		vector<string> lvec2; // for faces which also need a '/' split
		MeshData current; // here
		current.position = vec3(0.f);
		current.normal = vec3(0.f);
		current.coord = vec2(0.f);
		switch (line[0])
		{
			case 'v':
				if (line[1] == 't')
				{
					line = line.substr(3, line.size()-1);
					lvec = Util::split(line, ' ');
					current.coord = vec2(atof(lvec[0].c_str()), atof(lvec[1].c_str()));
					coords.push_back(current);
				}
				else if (line[1] == 'n')
				{
					line = line.substr(3, line.size()-1);
					lvec = Util::split(line, ' ');
					current.normal = vec3(
						atof(lvec[0].c_str()),
						atof(lvec[1].c_str()),
						atof(lvec[2].c_str())
					);
					normals.push_back(current);
				}
				else
				{
					line = line.substr(2, line.size()-1);
					lvec = Util::split(line, ' ');
					current.position = vec3(
						atof(lvec[0].c_str()),
						atof(lvec[1].c_str()),
						atof(lvec[2].c_str())
					);
					vertices.push_back(current);
				}
				break;
			case 'f':
				line = line.substr(2,line.size()-1);
				lvec = Util::split(line, ' ');
				for (int i = 0; i<lvec.size(); i++)
				{
					if (line.find("/") != string::npos)
					{
						lvec2 = Util::split(lvec[i], '/');
						current.position = vertices[atoi(lvec2[0].c_str())-1].position;
						if (!lvec2[1].empty()) current.coord = coords[atoi(lvec2[1].c_str())-1].coord;
						if (lvec2.size() == 3) current.normal = normals[atoi(lvec2[2].c_str())-1].normal;
					}
					if (!vertices.empty())
						mesh.push_back(current);
				}
				break;
			default:
				Log::Print("Unhandled: " + line);
				break;
		}
	}
	// I'M SORRY FOR THIS IT IS TERRIBLE
	// read: todo: make it actually a vbo
	vbo_id = glGenLists(1);
	glNewList(vbo_id, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i<mesh.size(); i++)
	{
		// OBJ stores normals reversed
		glNormal3fv(vec3(0.f)-mesh[i].normal);
		glTexCoord2fv(mesh[i].coord);
		glVertex3fv(mesh[i].position);
	}
	glEnd();
	glEndList();

}

void OBJLoader::Draw()
{
	// yuck
	glCallList(vbo_id);
/*
	// future: state batching
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(vec3)+sizeof(vec2), &mesh[0].position.x);
	glNormalPointer(3, sizeof(vec3)+sizeof(vec2), &mesh[0].normal.x);
	glTexCoordPointer(2, GL_FLOAT, sizeof(vec3)*2, &mesh[0].coord.x);

	glDrawArrays(GL_TRIANGLES, 0, mesh.size());

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
*/
}

void OBJLoader::Delete()
{
	glDeleteLists(vbo_id, 1);
}
