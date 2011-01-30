#include <GL/glew.h>
#include "Sprite.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include "Logger.h"
#include "PNGFile.h"

Sprite::Sprite()
{
	m_shader.SetProjectionMatrix(Game->ProjectionMatrix);
	m_texture.width = m_texture.height = 1;
}

Sprite::~Sprite()
{
	mesh.Free();
	
	if (m_texture.ptr != 0)
		ResourceManager::FreeResource(m_texture);
}

void Sprite::Load(std::string _path)
{
	std::string path = FileManager::GetFile(_path);
	std::string ext = path.substr(path.size()-3, path.size());
	Log->Print("Loading \"%s\" (type = %s)", _path.c_str(), ext.c_str());
	
	if (ext.compare("png") == 0)
	{
		// XXX: All sprites could be using the same 1u square here.
		// Create a VBO (1u square)
		MeshData verts[4];
		float vertices[] = {
			-0.5, -0.5, 0, 0, 0, 0, 0, 0,
			-0.5,  0.5, 0, 0, 0, 0, 0, 1,
			0.5, -0.5, 0, 0, 0, 0, 1, 0,
			0.5,  0.5, 0, 0, 0, 0, 1, 1,
		};
		unsigned indices[] = {
			0, 1, 2,
			1, 2, 3
		};
		memcpy(&verts[0].Position.x, vertices, sizeof(vertices));
		mesh.Load(verts, indices, 4, 6);

		Texture tex;
		
		if (!ResourceManager::GetResource(path,tex))
		{
			PNGFile png;
			tex = png.Load(path);
			
			// register this so we don't load it again.
			ResourceManager::Add(tex);
		}
		
		if (tex.ptr != 0)
			m_texture = tex;
	}
	else
		Log->Print("Unknown file type: %s.", ext.c_str());
		
	QueueUpdate();
}

void Sprite::DrawPrimitives()
{
	// Draw!
	mesh.Draw();
}

// Lua
#include <SLB/SLB.hpp>
void Sprite_Binding()
{
	SLB::Class<Sprite>("Sprite")
	.constructor()
	.set("Load", &Sprite::Load)
	.set("addState", &Sprite::addState)
	.set("addChild", &Sprite::addChild)
	.set("setParent", &Sprite::setParent)
	.set("setPosition", &Sprite::setPosition)
	.set("setRotate", &Sprite::setRotation)
	.set("setScale", &Sprite::setScale)
	.set("setColor", &Sprite::setColor);
}
