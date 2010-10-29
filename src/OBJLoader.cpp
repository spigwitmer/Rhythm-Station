#include "OBJLoader.h"
#include "Logger.h"
#include "FileManager.h"
#include "StringUtils.h"
#include <cstdlib>
#include <math.h>

using namespace std;

OBJLoader::OBJLoader() : vbo_id(), name(0) { /* nada */ }

OBJLoader::~OBJLoader() {
	Log->Print("Cleaning up mesh " + name);
}

GLuint *OBJLoader::Load(string file) {
	timer.Touch();
	file = File->GetFile(file);
	if (!File->FileExists(file)) {
		Log->Print("File \"" + file + "\" not found.");
		return 0;
	}
	file = File->GetFileContents(file);

	// store OBJ stuff here before loading up the full mesh var.
	vector<MeshData> vertices;
	vector<MeshData> normals;
	vector<MeshData> coords;

	vector<string> data = split(file, '\n');
	for (int i = 0; i<data.size(); i++) {
		string line = chop(data[i], "#");
		if (line.empty())
			continue;
		vector<string> lvec;
		vector<string> lvec2; // for faces which also need a '/' split
		MeshData current; // here
		current.position = vec3(0.f);
		current.normal = vec3(0.f);
		current.coord = vec2(0.f);
		switch (line[0]) {
			case 'v':
				if (line[1] == 't') {
					line = line.substr(3, line.size()-1);
					lvec = split(line, ' ');
					current.coord = vec2(atof(lvec[0].c_str()), atof(lvec[1].c_str()));
					coords.push_back(current);
				}
				else if (line[1] == 'n') {
					line = line.substr(3, line.size()-1);
					lvec = split(line, ' ');
					current.normal = vec3(
						atof(lvec[0].c_str()),
						atof(lvec[1].c_str()),
						atof(lvec[2].c_str())
					);
					normals.push_back(current);
				} else {
					line = line.substr(2, line.size()-1);
					lvec = split(line, ' ');
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
				lvec = split(line, ' ');
				for (int i = 0; i<lvec.size(); i++) {
					if (line.find("/") != string::npos) {
						lvec2 = split(lvec[i], '/');
						current.position = vertices[atoi(lvec2[0].c_str())-1].position;
						if (!lvec2[1].empty()) current.coord = coords[atoi(lvec2[1].c_str())-1].coord;
						if (lvec2.size() == 3) current.normal = normals[atoi(lvec2[2].c_str())-1].normal;
					}
					if (!vertices.empty())
						mesh.push_back(current);
				}
				break;
			default:
				Log->Print("Unhandled: " + line);
				break;
		}
	}
	glGenBuffers(2, vbo_id);

	// load stuff up
	// return vbo+ibo
	// make sure normals are flipped due to obj format

	return vbo_id;
}
