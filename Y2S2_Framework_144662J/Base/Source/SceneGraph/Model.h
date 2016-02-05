#pragma once

#include "Vector3.h"
#include "GL\glew.h"
#include "..\Mesh.h"
#include "Mtx44.h"

class CModel
{
private:
	Vector3 vTopLeft, vBottomRight;
	float red, green, blue;
	
	// Handle to the Mesh
	Mesh* m_cModelMesh;
	Mesh** theArrayOfMeshes;
	int m_iCurrentResolution;

public:
	enum RESOLUTION_TYPE
	{
		RESOLUTION_LOW,
		RESOLUTION_MEDIUM,
		RESOLUTION_HIGH,
		NUM_RESOLUTIONS,
	};

	CModel(void);
	~CModel(void);

	void Init();
	void Init(Mesh* mesh, Vector3 topLeft, Vector3 bottomRight);
	void Draw(bool m_bLight = false);
	void SetColor(const float red, const float green, const float blue);

	// Get the top left of the bounding box of the instance
	Vector3 GetTopLeft(void);
	// Get the bottomright of the bounding box of the instance
	Vector3 GetBottomRight(void);
	
	int GetResolution();
	void SetResolution(const int resolutionType);
	
	Mesh* GetMesh();
};
