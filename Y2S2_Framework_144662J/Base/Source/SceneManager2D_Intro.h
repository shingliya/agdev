#pragma once
#include "SceneManager2D.h"

class CSceneManager2D_Intro : public CSceneManager2D
{
public:

	enum GEOMETRY_TYPE
	{
		GEO_SPLASH,
		NUM_GEOMETRY,
	};

	CSceneManager2D_Intro();
	CSceneManager2D_Intro(int m_window_width, int m_window_height);
	~CSceneManager2D_Intro();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Mesh* meshList[NUM_GEOMETRY];
};

