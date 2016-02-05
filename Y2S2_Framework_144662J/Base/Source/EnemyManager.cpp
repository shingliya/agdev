#include "EnemyManager.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

EnemyManager::EnemyManager()
{
	wave = 0;
	numOfSpawnsPerWave = 1;
	curNumOfSpawnsPerWave = 0;
	numOfEnemiesPerSpawn = 3;
	timeTillNextSpawn = 7;
	curTimeTillNextSpawn = timeTillNextSpawn;
}

EnemyManager::~EnemyManager(void)
{
	for(int i = 0; i < enemyList.size(); ++i)
	{
		if(enemyList[i])
			delete enemyList[i];
	}
}

vector<Enemy*>* EnemyManager::getEnemyList()
{
	return &enemyList;
}

int EnemyManager::getWave()
{
	return wave;
}

void EnemyManager::Init(Vector3 target, CSpatialPartition* SP, CProjectileManager* PM)
{
	this->target = target;
	this->SP = SP;
	this->PM = PM;
}

void EnemyManager::update(double dt)
{
	if(wave == 0 || (getNumOfActiveEnemies() == 0 && curNumOfSpawnsPerWave == numOfSpawnsPerWave))
	{
		curTimeTillNextSpawn = timeTillNextSpawn;
		curNumOfSpawnsPerWave = 0;

		wave++;
		if(wave % 3 == 0)
		{
			numOfSpawnsPerWave++;
			numOfEnemiesPerSpawn-=2;
		}
		else
		{
			numOfEnemiesPerSpawn++;
		}

		numOfEnemiesPerWave = numOfEnemiesPerSpawn * numOfSpawnsPerWave;
	}
	else
	{
		if (numOfSpawnsPerWave != curNumOfSpawnsPerWave)
		{
			curTimeTillNextSpawn -= dt;
			if(curTimeTillNextSpawn < 0)
			{
				for(int i = 0; i < numOfEnemiesPerSpawn; ++i)
				{
					spawnEnemy();
				}
				curTimeTillNextSpawn = timeTillNextSpawn * 1.5;
				curNumOfSpawnsPerWave++;
			}
		}
	}

	for(int i = 0; i < enemyList.size(); ++i)
	{
		if(enemyList[i]->active)
		{
			enemyList[i]->Update(dt); 
			if(enemyList[i]->state == Enemy::Shooting)
			{
				CProjectile* temp = PM->AddRayProjectile(enemyList[i]->GetPos(), enemyList[i]->GetDir(), 50.f);
				temp->rotateY = Vector3(0, 0, 1).GetAngle(Vector3(enemyList[i]->GetDir().x, 0, enemyList[i]->GetDir().z).Normalized());
				temp->rotateXZ = Vector3(0, -1, 0).GetAngle(enemyList[i]->GetDir().Normalized()) + 90;
				temp->right = Vector3(Vector3(enemyList[i]->GetDir().x, 0, enemyList[i]->GetDir().z).Normalized()).Cross(Vector3(0, 1, 0));
			}
		}
	}
}

int EnemyManager::getNumOfActiveEnemies()
{
	int ret = 0;
	for(int i = 0; i < enemyList.size(); ++i)
	{
		if(enemyList[i]->active == true)
		{
			ret++;
		}
	}
	return ret;
}

void EnemyManager::spawnEnemy()
{
	int curIndex = 0;
	for(int i = 0; i < enemyList.size(); ++i)
	{
		if(enemyList[i]->active == true)
		{
			curIndex++;
		}
		else
			break;
	}

	if(curIndex >= enemyList.size())
	{
		enemyList.push_back(new Enemy);

		Vector3 temp = RandPosSpawn(750,250,650,350);
		float tempf = Math::RandFloatMinMax(10, 20);
		enemyList.back()->Init(temp,
			target,
			Math::RandFloatMinMax(100 + 100 * (wave/10), 200 + 100 * (wave/10)),
			Math::RandFloatMinMax(50, 150),
			Math::RandFloatMinMax(5, 8));

		CModel* newModel = new CModel();
		Mesh* tempMesh = new Mesh("mesh");
		tempMesh = MeshBuilder::GenerateOBJ("body", "OBJ//enemy.obj");
		tempMesh->textureID = LoadTGA("Image//enemy.tga");
		newModel->Init(tempMesh, Vector3(0.5, 0.5, 0.5), Vector3(-0.5, -0.5, -0.5));

		enemyList.back()->SetNode(new CTransform(0, 0, 0), newModel);
		enemyList.back()->ApplyTranslate(temp.x, temp.y, temp.z);
		enemyList.back()->ApplyScale(tempf, tempf, tempf);
		
		enemyList.back()->active = true;

		SP->AddObject(enemyList.back());
	}
	else
	{
		Vector3 temp = RandPosSpawn(750,250,650,350);
		float tempf = Math::RandFloatMinMax(10, 15);
		enemyList[curIndex]->Init(temp,
			target,
			Math::RandFloatMinMax(100 + 100 * (wave/10), 200 + 100 * (wave/10)),
			Math::RandFloatMinMax(80, 120),
			Math::RandFloatMinMax(5, 8));

		CModel* newModel = new CModel();
		Mesh* tempMesh = new Mesh("mesh");
		tempMesh = MeshBuilder::GenerateOBJ("body", "OBJ//enemy.obj");
		tempMesh->textureID = LoadTGA("Image//enemy.tga");
		newModel->Init(tempMesh, Vector3(0.5, 0.5, 0.5), Vector3(-0.5, -0.5, -0.5));

		enemyList[curIndex]->SetNode(new CTransform(0, 0, 0), newModel);
		enemyList[curIndex]->ApplyTranslate(temp.x, temp.y, temp.z);
		enemyList[curIndex]->ApplyScale(tempf, tempf, tempf);

		enemyList[curIndex]->active = true;

		SP->AddObject(enemyList[curIndex]);
	}
}

Vector3 EnemyManager::RandPosSpawn(float higherOutterRing, float lowerOutterRing, float higherInnerRing, float lowerInnerRing)
{
	float x, y, z;
	x = y = z = 0;

	while(x > higherOutterRing || x < lowerOutterRing || x < higherInnerRing && x > lowerInnerRing)
	{
		x = Math::RandFloatMinMax(lowerOutterRing, higherOutterRing);
	}
	while(y > higherOutterRing || y < lowerOutterRing || y < higherInnerRing && y > lowerInnerRing)
	{
		y = Math::RandFloatMinMax(lowerOutterRing, higherOutterRing);
	}
	while(z > higherOutterRing || z < lowerOutterRing || z < higherInnerRing && z > lowerInnerRing)
	{
		z = Math::RandFloatMinMax(lowerOutterRing, higherOutterRing);
	}

	return Vector3(x, y, z);
}