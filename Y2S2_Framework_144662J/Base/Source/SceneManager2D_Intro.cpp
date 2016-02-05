#include "SceneManager2D_Intro.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

CSceneManager2D_Intro::CSceneManager2D_Intro()
{
}

CSceneManager2D_Intro::CSceneManager2D_Intro(int m_window_width, int m_window_height)
:
CSceneManager2D(m_window_width, m_window_height)
{
}

CSceneManager2D_Intro::~CSceneManager2D_Intro()
{
}

void CSceneManager2D_Intro::Init()
{
	CSceneManager2D::Init();
	meshList[GEO_SPLASH] = MeshBuilder::Generate2DMesh("GEO_SPLASH", Color(1, 1, 1), 0, 0, m_window_width, m_window_height);
	meshList[GEO_SPLASH]->textureID = LoadTGA("Image//blaze.tga");
}

void CSceneManager2D_Intro::Update(double dt)
{
	CSceneManager2D::Update(dt);
}

void CSceneManager2D_Intro::Render()
{
	CSceneManager2D::Render();

	modelStack.PushMatrix();
	modelStack.Scale(800, 600, 1);
	RenderMeshIn2D(meshList[GEO_SPLASH], false);
	modelStack.PopMatrix();

	//On screen text
	//RenderTextOnScreen(CSceneManager2D::meshList[GEO_TEXT], "hello", Color(0, 1, 0), 10, 6, 6);
}

void CSceneManager2D_Intro::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	CSceneManager2D::Exit();
}