#include <GL/glew.h>
#include "Sprite.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "managers/FileManager.h"
#include "utils/Logger.h"
#include "PNGFile.h"

Sprite::Sprite()
{
	m_shader.setProjectionMatrix(Game->ProjectionMatrix);
	m_texture.width = m_texture.height = 1;
}

Sprite::~Sprite()
{
	mesh.Free();
	
	if (m_texture.ptr != 0)
		ResourceManager::GetSingleton()->FreeResource(m_texture);
}

void Sprite::Load(std::string _path)
{
	if (!FileManager::FileExists(FileManager::GetFile(_path), "png")) {
		Log->Print("File \"%s\" not found or had invalid extension.", _path.c_str());
		return;
	}
	std::string path = FileManager::GetFile(_path);
	std::string ext = path.substr(path.size()-3, path.size());
	Log->Print("Loading \"%s\"", _path.c_str(), ext.c_str());

	m_texture.Load(path);
	SetSize(m_texture.width, m_texture.height, 1.0);

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
	.set("Register", &Sprite::Register)
	.set("addState", &Sprite::AddState)
	.set("addChild", &Sprite::AddChild)
	.set("setAlign", &Sprite::SetAlign)
	.set("setParent", &Sprite::SetParent)
	.set("setPosition", &Sprite::SetPosition)
	.set("setRotate", &Sprite::SetRotation)
	.set("setScale", &Sprite::SetScale)
	.set("setSize", &Sprite::SetSize)
	.set("getWidth", &Sprite::GetWidth)
	.set("getHeight", &Sprite::GetHeight)
	.set("setColor", &Sprite::SetColor)
	.set("Update", &Sprite::Update)
	.set("Draw", &Sprite::Draw);
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
