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

struct PolyQuad {
	float v1, v2, v3, v4;
};

float max_zoom;

// hold verts
vector<Vertex>	verts;
vector<vec2>		uv;
vector<Vertex>	verts_quad;

// hold polygons
vector<PolyTri>	tris;
vector<PolyQuad>	quads;

ShaderLoader *shader = NULL;

void obj_load(string _path)
{
	shader = new ShaderLoader();
	shader->Load("harmonics.vert","harmonics.frag");
	/*
	 * OBJ Format:
	 *
	 * o Monkey # object named Monkey
	 * v 0.437500 0.164063 0.765625 vertex x y z
	 */
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
		PolyQuad quad;
		if (lines[i].find("v") != string::npos) {
			if (lines[i].find("t") != string::npos) {
//				Log::Print("uv");
			}
			else if (lines[i].find("n") != string::npos) {
//				Log::Print("normal");
			}
			else {
//				Log::Print("vertex");
				if (line.size() != 4) {
					Log::Print("[OBJLoader::Load] Invalid vertex.");
					break;
				}
				vert.x = atof(line[1].c_str());
				vert.y =atof(line[2].c_str());
				vert.z =atof(line[3].c_str());
				tmp_verts.push_back(vert);
			}
		}
		if (lines[i].find("f") != string::npos) {
			for (int i = 0; i<line.size(); i++) {
				line[i] = chop(line[i], "/");
			}
			// triangle
			if (line.size() == 4) {
				tri.v1 = atoi(line[1].c_str())-1;
				tri.v2 = atoi(line[2].c_str())-1;
				tri.v3 = atoi(line[3].c_str())-1;
				tris.push_back(tri);
			}
			// quad
			else if (line.size() == 5) {
				quad.v1 = atoi(line[1].c_str())-1;
				quad.v2 = atoi(line[2].c_str())-1;
				quad.v3 = atoi(line[3].c_str())-1;
				quad.v4 = atoi(line[4].c_str())-1;
				quads.push_back(quad);
			}
			// something else entirely
			else {
				// probably an ngon. don't bother.
				Log::Print("[OBJLoader::Load] NGons aren't supported.");
				break;
			}
		}
	}
	for (int i = 0; i<tris.size(); i++) {
		verts.push_back(tmp_verts[tris[i].v1]);
		verts.push_back(tmp_verts[tris[i].v2]);
		verts.push_back(tmp_verts[tris[i].v3]);
	}
	for (int i = 0; i<quads.size(); i++) {
		verts_quad.push_back(tmp_verts[quads[i].v1]);
		verts_quad.push_back(tmp_verts[quads[i].v2]);
		verts_quad.push_back(tmp_verts[quads[i].v3]);
		verts_quad.push_back(tmp_verts[quads[i].v4]);
	}
	vec3 max_size;
	for (int i=0; i<verts_quad.size(); i++) {
		max_size.x = max(max_size.x, verts_quad[i].x);
		max_size.y = max(max_size.y, verts_quad[i].y);
		max_size.z = max(max_size.z, verts_quad[i].z);
	}
	max_zoom = max(max_size.x, max_size.y);
	max_zoom = max(max_zoom, max_size.z);
	max_zoom = SCALE(max_zoom,0,max_zoom, 0, 100);
	printf("Max Zoom: %f\n",max_zoom);
//	coords.push_back(TexCoord(0.0f, 0.0f));
}

void obj_draw()
{
	shader->Bind();
//	glShadeModel(GL_SMOOTH);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_DEPTH);
	glPushMatrix();
		glRotatef(180, 1, 0, 0);
		glRotatef(30, 0, 1, 0);
		glScalef(max_zoom, max_zoom, max_zoom);
		// draw tris
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &verts[0].x);
		glDrawArrays(GL_TRIANGLES, 0, verts.size());
		glDisableClientState(GL_VERTEX_ARRAY);

		// draw quads
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(4, GL_FLOAT, 0, &verts_quad[0].x);
		glDrawArrays(GL_QUADS, 0, verts_quad.size());
		glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
	shader->Unbind();
}
