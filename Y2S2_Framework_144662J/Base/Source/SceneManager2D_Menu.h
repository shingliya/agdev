#pragma once
#include "SceneManager2D.h"
#include "ClickText.h"
#include <vector>

class CSceneManager2D_Menu: public CSceneManager2D
{
public:

	enum GEOMETRY_TYPE
	{
		GEO_BG,
		NUM_GEOMETRY,
	};

	CSceneManager2D_Menu();
	CSceneManager2D_Menu(int m_window_width, int m_window_height);
	~CSceneManager2D_Menu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	
	std::vector<ClickText*> clickTexts;
private:
	Mesh* meshList[NUM_GEOMETRY];
};