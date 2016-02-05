#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "SceneManager.h"
#include "SceneManager2D.h"
#include "GameStateManager.h"
#include "Introstate.h"
#include "Luala.h"

#define TYPE_OF_VIEW 3	// 2 = 2D, 3 = 3D


class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}

	static bool IsKeyPressed(unsigned short key);
	bool GetMouseUpdate();
	bool GetKeyboardUpdate();

	void Init();
	void Run();
	void Exit();

	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;
	static int Button_Left, Button_Middle, Button_Right;
	
	static bool exitbool;

	static void mouseMode(bool show);

private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	const static int m_window_deadzone = 1000;
	int m_window_width;
	int m_window_height;
	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

	// The handler for the scene
	#if TYPE_OF_VIEW == 3
		CSceneManager *scene;	// Use this for 3D gameplay
	#else
		CSceneManager2D *scene;	// Use this for 2D gameplay
	#endif

	// Game State Management
		CGameStateManager* GSM;
};
#endif