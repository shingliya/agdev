#include "SceneManager.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

CSceneManager::CSceneManager(void)
	: m_window_width(800)
	, m_window_height(600)
	, m_cSpatialPartition(NULL)
	, m_cProjectileManager(NULL)
	, m_EnemyManager(NULL)
	, m_defend(NULL)
	, gameover(false)
{
}

CSceneManager::CSceneManager(const int m_window_width, const int m_window_height)
	:m_player(NULL)
	, m_cSpatialPartition(NULL)
	, m_cProjectileManager(NULL)
	, m_EnemyManager(NULL)
	, m_defend(NULL)
	, gameover(false)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager::~CSceneManager(void)
{
	if (m_player)
	{
		delete m_player;
		m_player = NULL;
	}
	if (m_cSpatialPartition)
	{
		delete m_cSpatialPartition;
		m_cSpatialPartition = NULL;
	}
	if (m_cProjectileManager)
	{
		delete m_cProjectileManager;
		m_cProjectileManager = NULL;
	}
	if(m_EnemyManager)
	{
		delete m_EnemyManager;
	}
	if(m_defend)
	{
		delete m_defend;
	}
	for (int i = 0; i < clickTexts.size(); ++i)
	{
		if (clickTexts[i])
			delete clickTexts[i];
	}
}

void CSceneManager::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);


	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_RAY] = MeshBuilder::GenerateRay("ray", 10.0f);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//crosshair.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0.4, 1, 1), 18, 36, 0.5f);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	// spatial partition
	m_cSpatialPartition = new CSpatialPartition();
	m_cSpatialPartition->Init(200, 200, 5, 5);
	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); ++i)
	{
		for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			m_cSpatialPartition->SetGridMesh(i, j, MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f));
			m_cSpatialPartition->GetGridMesh(i, j)->textureID = LoadTGA("Image//grid.tga");
		}
	}

	m_cSpatialPartition->PrintSelf();

	meshList[GEO_PLAYERGRID] = MeshBuilder::GenerateQuad("BACK", Color(0.4, 1, 1), 1.f);
	meshList[GEO_ENEMYGRID] = MeshBuilder::GenerateQuad("BACK", Color(1, 0, 0), 1.f);

	PlayerInit();

	camera.Init(m_player->GetPosition() + 1, m_player->GetPosition(), m_player->GetUp(), m_player);



	m_cProjectileManager = new CProjectileManager();

	m_defend = new Defend;
	m_defend->Init(20, 10);
	CModel* newModel = new CModel();
	Mesh* tempMesh = new Mesh("mesh");
	tempMesh = MeshBuilder::GenerateOBJ("body", "OBJ//spark.obj");
	tempMesh->textureID = LoadTGA("Image//spark.tga");
	newModel->Init(tempMesh, Vector3(0.5, 0.5, 0.5), Vector3(-0.5, -0.5, -0.5));
	m_defend->SetNode(new CTransform(500, 500, 500), newModel);
	m_defend->ApplyScale(50, 50, 50);
	m_cSpatialPartition->AddObject(m_defend);

	m_EnemyManager = new EnemyManager();
	m_EnemyManager->Init(Vector3(500, 500, 500), m_cSpatialPartition,  m_cProjectileManager);

	clickTexts.push_back(new ClickText("Resume", 50, 100, 370));
	clickTexts.push_back(new ClickText("Quit", 50, 100, 270));

	state = S_Play;

	fireDelay = 0.7;
	currentFireDelay = 0;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	bLightEnabled = true;
}

void CSceneManager::Update(double dt)
{
	switch(state)
	{
	case S_Play:
		if(Application::IsKeyPressed('1'))
			glEnable(GL_CULL_FACE);
		if(Application::IsKeyPressed('2'))
			glDisable(GL_CULL_FACE);
		if(Application::IsKeyPressed('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(Application::IsKeyPressed('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if(Application::IsKeyPressed('5'))
		{
			lights[0].type = Light::LIGHT_POINT;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
		}
		else if(Application::IsKeyPressed('6'))
		{
			lights[0].type = Light::LIGHT_DIRECTIONAL;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
		}
		else if(Application::IsKeyPressed('7'))
		{
			lights[0].type = Light::LIGHT_SPOT;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
		}
		else if(Application::IsKeyPressed('8'))
		{
			bLightEnabled = true;
		}
		else if(Application::IsKeyPressed('9'))
		{
			bLightEnabled = false;
		}

		if(Application::IsKeyPressed('I'))
			lights[0].position.z -= (float)(10.f * dt);
		if(Application::IsKeyPressed('K'))
			lights[0].position.z += (float)(10.f * dt);
		if(Application::IsKeyPressed('J'))
			lights[0].position.x -= (float)(10.f * dt);
		if(Application::IsKeyPressed('L'))
			lights[0].position.x += (float)(10.f * dt);
		if(Application::IsKeyPressed('O'))
			lights[0].position.y -= (float)(10.f * dt);
		if(Application::IsKeyPressed('P'))
			lights[0].position.y += (float)(10.f * dt);

		m_player->Update(dt);
		//camera.Update(dt);
		camera.UpdatePostion(dt);

		fps = (float)(1.f / dt);

		m_cSpatialPartition->Update(camera.position);

		currentFireDelay+=dt;
		m_cProjectileManager->Update(dt);

		Vector3 ProjectilePosition;
		Vector3 ProjectilePosition_End;
		for (int i = 0; i<m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
		{
			if (m_cProjectileManager->IsActive(i))
			{
				ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();

				if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_DISCRETE)
				{
					// Destroy the projectile after collision
					CSceneNode* temp = m_cSpatialPartition->CheckForCollision(ProjectilePosition);
					if (temp != NULL)
					{
						m_cProjectileManager->RemoveProjectile(i);

						if(dynamic_cast<Enemy*>(temp) != NULL)
						{
							m_cSpatialPartition->theGrid[temp->currentGrid1]->RemoveObjectByAddress(temp);
							m_cSpatialPartition->theGrid[temp->currentGrid2]->RemoveObjectByAddress(temp);
							temp->active = false;
							m_EnemyManager->numOfEnemiesPerWave--;
						}
						else if(dynamic_cast<Defend*>(temp) != NULL)
						{
							dynamic_cast<Defend*>(temp)->GetHit();
							if (!temp->active)
							{
								m_cSpatialPartition->theGrid[temp->currentGrid1]->RemoveObjectByAddress(temp);
								m_cSpatialPartition->theGrid[temp->currentGrid2]->RemoveObjectByAddress(temp);
								gameover = true;
							}
						}
					}
				}
				else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_RAY)
				{
					ProjectilePosition_End = ProjectilePosition + m_cProjectileManager->theListOfProjectiles[i]->GetDirection()
						* m_cProjectileManager->theListOfProjectiles[i]->GetLength();

					// Destroy the ray projectile after collision
					CSceneNode* temp = m_cSpatialPartition->CheckForCollision(ProjectilePosition, ProjectilePosition_End);
					if (temp != NULL)
					{
						if(dynamic_cast<Defend*>(temp) != NULL)
						{
							m_cProjectileManager->RemoveProjectile(i);

							dynamic_cast<Defend*>(temp)->GetHit();
							if (!temp->active)
							{
								m_cSpatialPartition->theGrid[temp->currentGrid1]->RemoveObjectByAddress(temp);
								m_cSpatialPartition->theGrid[temp->currentGrid2]->RemoveObjectByAddress(temp);
								gameover = true;
							}
						}
					}
				}
			}
		}
		if(!gameover)
			m_EnemyManager->update(dt);
		m_defend->Update(dt);
		break;
	}
}

/********************************************************************************
Update Camera position
********************************************************************************/
void CSceneManager::UpdateCameraStatus(const unsigned char key, const bool status)
{
	camera.UpdateStatus(key, status);
}

void CSceneManager::UpdatePlayerStatus(const unsigned char key, const bool status)
{
	m_player->UpdateMovement(key, status);
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void CSceneManager::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE && currentFireDelay > fireDelay && m_player->Zoom)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
		Vector3 bulletpos = (m_player->GetPosition() + m_player->GetRight()*0.7) + m_player->GetDirection() *2;
		CProjectile* temp = m_cProjectileManager->Add3rdPersonProjectile(bulletpos, (camera.target - bulletpos).Normalized(), camera.position + (camera.target - camera.position).Normalized() * 70, (camera.target - camera.position).Normalized(), 250.f);
		temp->SetType(CProjectile::PROJ_TYPE_DISCRETE);
		currentFireDelay = 0;
	}
	/*else if (key == WA_FIRE_SECONDARY)
	{
	m_cProjectileManager->AddRayProjectile(camera.position, (camera.target - camera.position).Normalized(), 50.f);
	}*/
}

/********************************************************************************
Start the rendering process by calling PreRendering()
********************************************************************************/
void CSceneManager::PreRendering(Mtx44 matrix, bool enableLight, Mesh* mesh)
{
	modelStack.PushMatrix();
	modelStack.MultMatrix(matrix);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
}

void CSceneManager::PreRendering(Mtx44 matrix, bool enableLight, Mesh* mesh, Mtx44 staticMatrix)
{
	modelStack.PushMatrix();
	modelStack.MultMatrix(matrix);
	modelStack.MultMatrix(staticMatrix);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
}

/********************************************************************************
Complete the rending process by calling the PostRendering()
********************************************************************************/
void CSceneManager::PostRendering()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	modelStack.PopMatrix();
}

/********************************************************************************
Render text onto the screen with reference position in the middle of the image
********************************************************************************/
void CSceneManager::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.8f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
Render text onto the screen
********************************************************************************/
void CSceneManager::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation( (i*0.5f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
Render a mesh in 2D
********************************************************************************/
void CSceneManager::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

/********************************************************************************
Render a mesh
********************************************************************************/
void CSceneManager::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/********************************************************************************
Render mobile objects
********************************************************************************/
void CSceneManager::RenderGUI()
{
	// Render the crosshair
	if(m_player->Zoom)
		RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 25, 5, 575);

	/*ss.str("");
	ss << "Prectiles: " << m_cProjectileManager->NumOfActiveProjectile;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 26);*/

	ss.str("");
	ss << "wave:" << m_EnemyManager->getWave();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0),40, 350, 562);

	ss.str("");
	ss << "mobs left:" << m_EnemyManager->numOfEnemiesPerWave;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0),30, 325, 532);

	if(gameover && state != S_Pause)
	{
		ss.str("");
		ss << "GAMEOVER";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0),100, 200, 250);
	}

	if(state == S_Pause)
	{
		for (int i = 0; i < clickTexts.size(); ++i)
		{
			if (clickTexts[i]->GetMouseover())
				RenderTextOnScreen(meshList[GEO_TEXT], clickTexts[i]->GetName(), Color(1, 0, 0), clickTexts[i]->GetSize(), clickTexts[i]->GetX(), clickTexts[i]->GetY());
			else
				RenderTextOnScreen(meshList[GEO_TEXT], clickTexts[i]->GetName(), Color(1, 1, 0), clickTexts[i]->GetSize(), clickTexts[i]->GetX(), clickTexts[i]->GetY());
		}
	}
}

/********************************************************************************
Render mobile objects
********************************************************************************/
void CSceneManager::RenderMobileObjects()
{
	// Render LightBall
	modelStack.PushMatrix();
	modelStack.Translate(500, 500, 500);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	for(int i = 0; i < m_EnemyManager->getEnemyList()->size(); ++i)
	{
		if(m_EnemyManager->getEnemyList()->at(i)->active == true)
		{
			m_EnemyManager->getEnemyList()->at(i)->Draw(this);
		}
	}
	if(m_defend->active)
		m_defend->Draw(this);
	{
		float x, y, z;
		m_defend->getTransform()->GetOffset(x, y, z);
		std::ostringstream ss;
		ss << "HP:" << m_defend->getHealth();

		modelStack.PushMatrix();
		modelStack.Translate(x - m_player->GetRight().x * 23, y + 50, z - m_player->GetRight().z * 23);
		modelStack.Rotate(Vector3(0, -1, 0).GetAngle(Vector3(m_player->GetDirection().x, m_player->GetDirection().y, m_player->GetDirection().z)) - 90, m_player->GetRight().x, 0, m_player->GetRight().z);
		modelStack.Rotate(Vector3(0, 0, -1).GetAngle(Vector3(m_player->GetDirection().x, 0, m_player->GetDirection().z)), 0, 1, 0);
		modelStack.Scale(20, 20, 20);
		float red = 1.f - ((float)m_defend->getHealth()/20);
		float green = (float)m_defend->getHealth()/20;
		RenderText(meshList[GEO_TEXT], ss.str(), Color(red, green, 0));
		modelStack.PopMatrix();
	}

	//render player
	m_player->Draw2(this);

	Vector3 ProjectilePosition;
	for (int i = 0; i < m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
	{
		if (m_cProjectileManager->IsActive(i))
		{
			ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();
			modelStack.PushMatrix();
			modelStack.Translate(ProjectilePosition.x, ProjectilePosition.y, ProjectilePosition.z);
			if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_DISCRETE)
			{
				RenderMesh(meshList[GEO_SPHERE], false);
			}
			else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_RAY)
			{
				glLineWidth(5.0f);
				modelStack.PushMatrix();
				modelStack.Rotate(m_cProjectileManager->theListOfProjectiles[i]->rotateXZ, m_cProjectileManager->theListOfProjectiles[i]->right.x,  m_cProjectileManager->theListOfProjectiles[i]->right.y, m_cProjectileManager->theListOfProjectiles[i]->right.z);
				modelStack.Rotate(m_cProjectileManager->theListOfProjectiles[i]->rotateY, 0, 1, 0);
				RenderMesh(meshList[GEO_RAY], false);
				modelStack.PopMatrix();
				glLineWidth(1.0f);
			}
			modelStack.PopMatrix();
		}

	}
}

/********************************************************************************
Render the lights in this scene
********************************************************************************/
void CSceneManager::RenderFixedObjects()
{
	//RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	//RenderText(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0));
	RenderText(meshList[GEO_TEXT], "DM2240 AGDEV", Color(0, 1, 0));
	modelStack.PopMatrix();
}

/********************************************************************************
Render the lights in this scene
********************************************************************************/
void CSceneManager::RenderLights()
{
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
}

/********************************************************************************
Render the ground in this scene
********************************************************************************/
void CSceneManager::RenderGround()
{
	/*modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);

	for (int x=0; x<10; x++)
	{
	for (int z=0; z<10; z++)
	{
	modelStack.PushMatrix();
	modelStack.Translate(x-5.0f, z-5.0f, 0.0f);
	if ( ((x*9+z) % 2) == 0)
	RenderMesh(meshList[GEO_GRASS_DARKGREEN], false);
	else
	RenderMesh(meshList[GEO_GRASS_LIGHTGREEN], false);
	modelStack.PopMatrix();
	}
	}
	modelStack.PopMatrix();*/

	// Render the Spartial Partitions

	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); ++i)
	{
		for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_cSpatialPartition->xGridSize*i + m_cSpatialPartition->xGridSize / 2, -9, m_cSpatialPartition->yGridSize*j + m_cSpatialPartition->yGridSize / 2);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(m_cSpatialPartition->xGridSize, m_cSpatialPartition->yGridSize, 1); 
			
			if (m_player->currentGrid1 == i*5 + j || m_player->currentGrid2 == i*5 + j)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.95, 0.95, 1); 
				RenderMesh(meshList[GEO_PLAYERGRID], false);
				modelStack.PopMatrix();
			}
			else if (m_cSpatialPartition->theGrid[i*5 + j]->GetListOfObject().size() > 0)
				if(m_defend->currentGrid1 != i*5 + j && m_cSpatialPartition->theGrid[i*5 + j]->GetListOfObject().size() != 1)
				{}
				else
				{
					modelStack.PushMatrix();
				modelStack.Scale(0.95, 0.95, 1);
					RenderMesh(meshList[GEO_ENEMYGRID], false);
				modelStack.PopMatrix();
				}
			else
				RenderMesh(m_cSpatialPartition->GetGridMesh(i, j), false);;
			modelStack.PopMatrix();
		}
	}/*
	 for (int i = 0; i < 9; ++i)
	 {
	 cout << m_cSpatialPartition->theGrid[i]->GetListOfObject().size() << ',';
	 }
	 cout << endl;*/
}

/********************************************************************************
Render the skybox in this scene
********************************************************************************/
void CSceneManager::RenderSkybox()
{
	//left
	modelStack.PushMatrix();
	modelStack.Translate(500, 0, 500);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 0, 500);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 0, 500);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 0, 500);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 0, 500);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 0, 500);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

/********************************************************************************
Render this scene
********************************************************************************/
void CSceneManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(	camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z );

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderLights();
	RenderGround();
	RenderSkybox();
	RenderFixedObjects();
	RenderMobileObjects();

	RenderGUI();
}

/********************************************************************************
Exit process for this scene
********************************************************************************/
void CSceneManager::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void CSceneManager::PlayerInit()
{
	CModel* newModel = new CModel();
	Mesh* tempMesh = new Mesh("mesh");
	m_player = new Player();

	tempMesh = MeshBuilder::GenerateOBJ("body", "OBJ//body.obj");
	tempMesh->textureID = LoadTGA("Image//Iron_Man_Skin_2.tga");
	newModel->Init(tempMesh, Vector3(1, 1, 1), Vector3(-1, -1, -1));

	m_player->SetNode(new CTransform(m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z), newModel);

	newModel = new CModel();
	tempMesh = new Mesh("mesh");

	tempMesh = MeshBuilder::GenerateOBJ("head", "OBJ//head.obj");
	tempMesh->textureID = LoadTGA("Image//Iron_Man_Skin_2.tga");
	newModel->Init(tempMesh, Vector3(1, 1, 1), Vector3(-1, -1, -1));

	cout << m_player->AddChild(new CTransform(0, 1.5, 0), newModel) << endl;

	newModel = new CModel();
	tempMesh = new Mesh("mesh");

	tempMesh = MeshBuilder::GenerateOBJ("head", "OBJ//arm-right.obj");
	tempMesh->textureID = LoadTGA("Image//Iron_Man_Skin_2.tga");
	newModel->Init(tempMesh, Vector3(1, 1, 1), Vector3(-1, -1, -1));

	m_player->id_rightHand = m_player->AddChild(new CTransform(-0.75, 0, 0), newModel);

	newModel = new CModel();
	tempMesh = new Mesh("mesh");

	tempMesh = MeshBuilder::GenerateOBJ("head", "OBJ//arm-left.obj");
	tempMesh->textureID = LoadTGA("Image//Iron_Man_Skin_2.tga");
	newModel->Init(tempMesh, Vector3(1, 1, 1), Vector3(-1, -1, -1));

	m_player->id_leftHand = m_player->AddChild(new CTransform(0.75, 0, 0), newModel);

	newModel = new CModel();
	tempMesh = new Mesh("mesh");

	tempMesh = MeshBuilder::GenerateOBJ("head", "OBJ//leg-right.obj");
	tempMesh->textureID = LoadTGA("Image//Iron_Man_Skin_2.tga");
	newModel->Init(tempMesh, Vector3(1, 1, 1), Vector3(-1, -1, -1));

	m_player->id_rightLeg = m_player->AddChild(new CTransform(-0.25, -2, 0), newModel);

	newModel = new CModel();
	tempMesh = new Mesh("mesh");

	tempMesh = MeshBuilder::GenerateOBJ("head", "OBJ//leg-left.obj");
	tempMesh->textureID = LoadTGA("Image//Iron_Man_Skin_2.tga");
	newModel->Init(tempMesh, Vector3(1, 1, 1), Vector3(-1, -1, -1));

	m_player->id_leftLeg = m_player->AddChild(new CTransform(0.25, -2, 0), newModel);

	m_cSpatialPartition->AddObject(m_player);
}