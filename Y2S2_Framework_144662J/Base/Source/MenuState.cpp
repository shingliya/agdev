#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "introstate.h"
#include "playstate.h"
#include "gamestate.h"
#include "menustate.h"
#include "Application.h"

CMenuState CMenuState::theMenuState;

void CMenuState::Init()
{
#if GSM_DEBUG_MODE
	//cout << "CMenuState::Init\n" << endl;
#endif
	scene = new CSceneManager2D_Menu(800, 600);
	scene->Init();
}

void CMenuState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	//cout << "CMenuState::Init\n" << endl;
#endif
	scene = new CSceneManager2D_Menu(width, height);
	scene->Init();
}

void CMenuState::Cleanup()
{
#if GSM_DEBUG_MODE
	//cout << "CMenuState::Cleanup\n" << endl;
#endif
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CMenuState::Pause()
{
#if GSM_DEBUG_MODE
	//cout << "CMenuState::Pause\n" << endl;
#endif
}

void CMenuState::Resume()
{
#if GSM_DEBUG_MODE
	//cout << "CMenuState::Resume\n" << endl;
#endif
}

void CMenuState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//int m_iUserChoice = -1;

	//do {
	//	cout << "CMenuState: Choose one <0> Go to Intro State, <1> Go to Play State : " ;
	//	cin >> m_iUserChoice;
	//	cin.get();

	//	switch (m_iUserChoice) {
	//		case 0:
	//			theGSM->ChangeState( CIntroState::Instance() );
	//			break;

	//		case 1:
	//			theGSM->ChangeState( CPlayState::Instance() );
	//			break;
	//		default:
	//			cout << "Invalid choice!\n";
	//			m_iUserChoice = -1;
	//			break;
	//	}
	//} while (m_iUserChoice == -1);
#endif
}

void CMenuState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	/*int m_iUserChoice = -1;

	do {
		cout << "CMenuState: Choose one <0> Go to Intro State, <1> Go to Play State : " ;
		cin >> m_iUserChoice;
		cin.get();

		switch (m_iUserChoice) {
			case 0:
				theGSM->ChangeState( CIntroState::Instance() );
				break;

			case 1:
				theGSM->ChangeState( CPlayState::Instance() );
				break;
			default:
				cout << "Invalid choice!\n";
				m_iUserChoice = -1;
				break;
		}
	} while (m_iUserChoice == -1);*/
#endif
}

void CMenuState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
	/*int m_iUserChoice = -1;

	do {
		cout << "CMenuState: Choose one <0> Go to Intro State, <1> Go to Play State : " ;
		cin >> m_iUserChoice;
		cin.get();

		switch (m_iUserChoice) {
			case 0:
				theGSM->ChangeState( CIntroState::Instance() );
				break;

			case 1:
				theGSM->ChangeState( CPlayState::Instance() );
				break;
			default:
				cout << "Invalid choice!\n";
				m_iUserChoice = -1;
				break;
		}
	} while (m_iUserChoice == -1);*/
#endif
	for (int i = 0; i < scene->clickTexts.size(); ++i)
	{
		scene->clickTexts[i]->CheckMouseOver(mouse_x, mouse_y);

		if (scene->clickTexts[i]->GetName() == "Start" && scene->clickTexts[i]->GetMouseover() && button_Left == 1)
		{
			theGSM->ChangeState(CPlayState::Instance());
			break;
		}
		if (scene->clickTexts[i]->GetName() == "Exit" && scene->clickTexts[i]->GetMouseover() && button_Left == 1)
		{
			Application::exitbool = true;
			break;
		}
	}
}

void CMenuState::Update(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	//cout << "CMenuState::Update\n" << endl;
#endif
	scene->Update(0.16667);
}

void CMenuState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	scene->Update(m_dElapsedTime);
}

void CMenuState::Draw(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	//cout << "CMenuState::Draw\n" << endl;
#endif
	scene->Render();
}
