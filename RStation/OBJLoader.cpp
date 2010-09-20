#include <GL/glew.h>
#include "OBJLoader.h"
#include "RSUtil.h"
#include "FileManager.h"
#include "Type.h"
#include "Primitives.h"
#include "MathUtils.h"
#include "ShaderLoader.h"

using namespace std;
using namespace Util;

struct PolyTri {
	float v1, v2, v3;
};

float max_zoom;

// hold verts
vector<Vertex> verts;
vector<PolyTri> tris;

ShaderLoader *shader = NULL;

void obj_load(string _path)
{
	shader = new ShaderLoader();
	shader->Load("harmonics.vert","harmonics.frag");

	_path = FileManager::GetFile(_path);
	string file = FileManager::GetFileContents(_path);
	vector<Vertex> tmp_verts;
	vector<string> lines = split(file,'\n');
	for (int i = 0; i<lines.size(); i++) {
		// if this line is a comment, skip it.
		if (lines[i][0] == '#')
			continue;

		// strip comments
		lines[i] = chop(lines[i],"#");

		// strip any comments later in the line
		vector<string> line = split(lines[i],' ');
		Vertex vert;
		PolyTri tri;
		if (lines[i][0] == 'v') {
			if (line.size() != 4) {
				Log::Print("[OBJLoader::Load] Invalid vertex.");
				break;
			}
			vert.x = atof(line[1].c_str());
			vert.y =atof(line[2].c_str());
			vert.z =atof(line[3].c_str());
			tmp_verts.push_back(vert);
		}
		if (lines[i][0] == 'f') {
			// triangle
			if (line.size() == 4) {
				tri.v1 = atoi(line[1].c_str())-1;
				tri.v2 = atoi(line[2].c_str())-1;
				tri.v3 = atoi(line[3].c_str())-1;
				tris.push_back(tri);
			}
			// something else entirely
			else {
				// probably an ngon. don't bother.
				Log::Print("[OBJLoader::Load] Quads and NGons aren't supported yet.");
				break;
			}
		}
	}
	for (int i = 0; i<tris.size(); i++) {
		verts.push_back(tmp_verts[tris[i].v1]);
		verts.push_back(tmp_verts[tris[i].v2]);
		verts.push_back(tmp_verts[tris[i].v3]);
	}
	vec3 max_size;
	for (int i=0; i<verts.size(); i++) {
		max_size.x = max(max_size.x, verts[i].x);
		max_size.y = max(max_size.y, verts[i].y);
		max_size.z = max(max_size.z, verts[i].z);
	}
	max_zoom = max(max_size.x, max_size.y);
	max_zoom = max(max_zoom, max_size.z);
	max_zoom = SCALE(max_zoom, 0, max_zoom, 0, 100);
}

void obj_draw()
{
	shader->Bind();
	glShadeModel(GL_SMOOTH);
	glPushMatrix();
		glRotatef(180, 1, 0, 0);
		glScalef(max_zoom, max_zoom, max_zoom);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &verts[0].x);
		glDrawArrays(GL_TRIANGLES, 0, verts.size());
		glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
	shader->Unbind();
}
