#include "Model.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

#include <stdio.h>
#include "..\MeshBuilder.h"

CModel::CModel(void)
	: m_cModelMesh(NULL)
	, m_iCurrentResolution(RESOLUTION_HIGH)
	, theArrayOfMeshes(NULL)
{
	vTopLeft = Vector3( 1.0f, 1.0f, 1.0f );
	vBottomRight = Vector3( -1.0f, -1.0f, -1.0f );
	red = 128;
	green = 255;
	blue = 0;

	theArrayOfMeshes = new Mesh*[NUM_RESOLUTIONS];
	for (int i = 0; i < NUM_RESOLUTIONS; i++)
	{
		theArrayOfMeshes[i] = NULL;
	}

}

CModel::~CModel(void)
{
	if (m_cModelMesh != NULL)
	{
		delete m_cModelMesh;
		m_cModelMesh = NULL;
	}

	if (theArrayOfMeshes != NULL)
	{
		for (int i = 0; i < NUM_RESOLUTIONS; i++)
		{
			delete theArrayOfMeshes[i];
		}
		delete [] theArrayOfMeshes;
		theArrayOfMeshes = NULL;
	}
}

void CModel::Init()
{
	m_cModelMesh = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1);
}

void CModel::Init(Mesh* mesh, Vector3 topLeft, Vector3 bottomRight)
{
	m_cModelMesh = mesh;
	vTopLeft = topLeft;
	vBottomRight = bottomRight;
	//m_cModelMesh = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	
	theArrayOfMeshes[RESOLUTION_LOW] = MeshBuilder::GenerateSphere("low", Color(0, 0, 1), 6, 12, 0.5);
	theArrayOfMeshes[RESOLUTION_MEDIUM] = MeshBuilder::GenerateSphere("med", Color(0, 1, 0), 12, 24, 0.5);
	theArrayOfMeshes[RESOLUTION_HIGH] = MeshBuilder::GenerateSphere("hig", Color(1, 1, 0), 18, 36, 0.5);
}

void CModel::Draw(bool m_bLight)
{
	theArrayOfMeshes[m_iCurrentResolution]->Render();

	if (m_cModelMesh)
		m_cModelMesh->Render();
}

void CModel::SetColor(const float red, const float green, const float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

// Get the top left of the bounding box of the instance
Vector3 CModel::GetTopLeft(void)
{
	return Vector3(vTopLeft.x, vTopLeft.y, vTopLeft.z);
}

// Get the bottomright of the bounding box of the instance
Vector3 CModel::GetBottomRight(void)
{
	return Vector3(vBottomRight.x, vBottomRight.y, vBottomRight.z);
}

int CModel::GetResolution()
{
	return m_iCurrentResolution;
}

void CModel::SetResolution(const int resolutionType)
{
	if ((resolutionType >= RESOLUTION_LOW) && (resolutionType <= RESOLUTION_HIGH))
		m_iCurrentResolution = resolutionType;
}

Mesh* CModel::GetMesh()
{
	return m_cModelMesh;
}